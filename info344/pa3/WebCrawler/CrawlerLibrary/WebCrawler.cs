using Microsoft.WindowsAzure.Storage.Queue;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;
using System.Web;
using System.Diagnostics;
using Microsoft.VisualBasic.CompilerServices;
using Microsoft.VisualBasic;
using Microsoft.WindowsAzure.Storage;
using HtmlAgilityPack;

namespace CrawlerLibrary
{
    public class WebCrawler
    {
        private string userAgent;
        private Dictionary<string, RobotsParser> robots;
        private CloudQueue urlsQueue;
        private CloudTable indexTable;
        private HashSet<string> urlsCrawled;

        public WebCrawler(string userAgent, CloudQueue queue,
                          CloudTable table)
        {
            this.userAgent = userAgent;
            robots = new Dictionary<string, RobotsParser>();
            urlsQueue = queue;
            indexTable = table;
            urlsCrawled = new HashSet<string>();
        }

        public void InitializeCrawler(params string[] websites)
        {
            Reset();
            RobotsParser parser;
            foreach (var website in websites)
            {
                parser = new RobotsParser(userAgent);
                using (StreamReader reader = new StreamReader(GetStreamFromUrl(website + "/robots.txt")))
                {
                    parser.Parse(reader);
                    robots[website] = parser;
                }
            }
        }

        public void CrawlSitemaps()
        {
            ThreadPool.QueueUserWorkItem(PrivCrawlSitemaps, this);
        }

        private void PrivCrawlSitemaps(object context)
        {
            foreach (var website in robots)
            {
                if (website.Key == "www.bleacherreport.com")
                {
                    ParseSitemap("http://bleacherreport.com/sitemap/nba.xml");
                }
                else
                {
                    foreach (var sitemap in website.Value.SiteMaps)
                    {
                        ParseSitemap(sitemap);
                    }
                }
            }
        }

        public void Reset()
        {
            urlsQueue.Clear();
        }

        public bool CrawlNext()
        {
            string url = DequeueUrl();
            if (url == null)
                return false;

            ThreadPool.QueueUserWorkItem(ParsePageCallback, url);

            return true;
        }

        private void ParsePageCallback(object context)
        {
            string pageUrl = context.ToString();
            using (Stream htmlStream = GetStreamFromUrl(pageUrl))
            {
                if (htmlStream == null)
                    return;
                Trace.TraceInformation("Parsing '{0}'.", pageUrl);

                HtmlDocument doc = new HtmlDocument();
                doc.Load(htmlStream);
                var title = doc.DocumentNode.SelectSingleNode("//title");
                if (title == null)
                    title = doc.DocumentNode.SelectSingleNode("//h1");

                if (title == null || string.IsNullOrEmpty(title.InnerText))
                    Trace.TraceInformation("Page title not found for '{0}'.", pageUrl);
                else
                {
                    WebPageData pageData = new WebPageData(pageUrl, title.InnerText);
                    TableOperation insertOp = TableOperation.InsertOrReplace(pageData);
                    indexTable.Execute(insertOp);
                }

                var links = doc.DocumentNode.SelectNodes("//a");
                if (links != null)
                {
                    foreach (var linkTag in links)
                    {
                        EnqueueUrlIfValid(linkTag.GetAttributeValue("href", ""));
                    }
                }
            }
        }

        private void ParseSitemap(string sitemap)
        {
            Queue<string> urls = new Queue<string>();
            urls.Enqueue(sitemap);

            while (urls.Count > 0)
            {
                string current = urls.Dequeue();
                using (Stream xmlStream = GetStreamFromUrl(current))
                {
                    XmlDocument doc = new XmlDocument();
                    doc.Load(xmlStream);
                    if (doc.GetElementsByTagName("urlset").Count > 0)
                    {
                        foreach (XmlNode urlNode in doc.GetElementsByTagName("loc"))
                        {
                            EnqueueUrlIfValid(urlNode.InnerText);
                            Thread.Sleep(50);
                        }
                    }
                    else if (doc.GetElementsByTagName("sitemapindex").Count > 0)
                    {
                        string txt;
                        foreach (XmlNode urlNode in doc.GetElementsByTagName("loc"))
                        {
                            // TODO: Check date within last 2 months
                            txt = urlNode.InnerText;
                            if (txt.Contains("2015"))
                                urls.Enqueue(txt);
                        }
                    }
                    else
                    {
                        Trace.TraceError("The given XML sitemap does not contain urlset" +
                                         "or a sitmap index. Sitemap: " + sitemap);
                    }
                }
            }
        }

        private void EnqueueUrlIfValid(string url)
        {
            url = FixUrl(url);
            Uri uri;
            if (!Uri.TryCreate(url, UriKind.Absolute, out uri) || !IsValidUrl(uri))
                return;

            url = uri.GetComponents(UriComponents.HttpRequestUrl, UriFormat.Unescaped);
            lock (urlsCrawled)
            {
                if (urlsCrawled.Contains(url) || UrlCrawled(uri))
                    return;
                urlsCrawled.Add(url);
            }

            CloudQueueMessage msg = new CloudQueueMessage(url);
            urlsQueue.BeginAddMessage(msg, (IAsyncResult ar) =>
            {
                Trace.TraceInformation("Thread {1} added to queue: {0}", url, 
                                        Thread.CurrentThread.ManagedThreadId);
            }, this);
        }

        private string DequeueUrl()
        {
            CloudQueueMessage msg = urlsQueue.GetMessage();
            if (msg == null)
                return null;

            urlsQueue.DeleteMessageAsync(msg);
            return msg.AsString;
        }

        private bool IsValidUrl(Uri uri)
        {
            string host = uri.GetComponents(UriComponents.Host, UriFormat.Unescaped);
            string url = uri.GetComponents(UriComponents.HttpRequestUrl, UriFormat.Unescaped);

            if (Operators.LikeString(host, "*.bleacherreport.com", CompareMethod.Text) ||
                host == "bleacherreport.com")
            {
                lock (robots) return robots["www.bleacherreport.com"].IsUrlAllowed(url);
            }
            else if (Operators.LikeString(host, "*.cnn.com", CompareMethod.Text) ||
                     host == "cnn.com")
            {
                lock (robots) return robots["www.cnn.com"].IsUrlAllowed(url);
            }

            return false;
        }

        private bool UrlCrawled(Uri uri)
        {
            string url = uri.GetComponents(UriComponents.HttpRequestUrl, UriFormat.Unescaped);
            TableQuery<WebPageData> query = new TableQuery<WebPageData>()
                .Where(TableQuery.CombineFilters(
                        TableQuery.GenerateFilterCondition("RowKey", QueryComparisons.Equal, WebUtility.UrlEncode(url)),
                        TableOperators.And,
                        TableQuery.GenerateFilterCondition("PartitionKey", QueryComparisons.Equal, WebPageData.GetSimpleHost(url))));
            int count = indexTable.ExecuteQuery(query).Count();
            return count > 0;
        }

        private Stream GetStreamFromUrl(string url)
        {
            try
            {
                url = FixUrl(url);
                HttpWebRequest request = WebRequest.CreateHttp(url);
                request.UserAgent = userAgent;
                return request.GetResponse().GetResponseStream();
            }
            catch (WebException webExp)
            {
                Trace.TraceError("{0} - {1}: {2}", url, webExp.Status, webExp.Message);
                return null;
            }
        }

        private string FixUrl(string url)
        {
            if (url.StartsWith("http://") || url.StartsWith("https://"))
            {
                return url;
            }

            return "http://" + url;
        }
    }
}
