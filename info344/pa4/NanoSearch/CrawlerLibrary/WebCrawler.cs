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
using System.Text.RegularExpressions;

namespace CrawlerLibrary
{
    public class WebCrawler
    {
        private readonly Semaphore workLimiter = new Semaphore(150, 150);

        private string userAgent;
        private Dictionary<string, RobotsParser> robots;
        private CloudQueue urlsQueue;
        private CloudTable indexTable;
        private CloudTable errorTable;

        private HashSet<string> urlsFound;
        private Queue<string> lastTen;

        private bool stopLoading = false;

        public bool IsRunning
        {
            get
            {
                return stopLoading;
            }
        }
        public bool IsLoading { get; private set; }
        public int NumUrlsIndexed { get; private set; }
        public int NumUrlsCrawled { get; private set; }
        public int NumErrors { get; set; }

        public WebCrawler(string userAgent, CloudQueue queue,
                          CloudTable table, CloudTable errorTable)
        {
            this.userAgent = userAgent;
            robots = new Dictionary<string, RobotsParser>();
            urlsQueue = queue;
            indexTable = table;
            this.errorTable = errorTable;
            urlsFound = new HashSet<string>();
            lastTen = new Queue<string>(10);
            IsLoading = false;
        }

        public void InitializeCrawler(params string[] websites)
        {
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
            if (stopLoading)
                throw new InvalidOperationException("Cannot crawl before resetting.");
            else if (IsLoading)
                throw new InvalidOperationException("Already crawling.");

            ThreadPool.QueueUserWorkItem(PrivCrawlSitemaps, this);
        }

        private void PrivCrawlSitemaps(object context)
        {
            IsLoading = true;
            foreach (var website in robots)
            {
                if (stopLoading)
                    break;

                if (website.Key == "www.bleacherreport.com")
                {
                    ParseSitemap("http://bleacherreport.com/sitemap/nba.xml");
                }
                else
                {
                    foreach (var sitemap in website.Value.SiteMaps)
                    {
                        if (stopLoading)
                            break;
                        ParseSitemap(sitemap);
                    }
                }
            }
            IsLoading = false;
        }

        public void Reset()
        {
            urlsQueue.Clear();
            urlsFound.Clear();
            lastTen.Clear();
            stopLoading = false;
            NumUrlsIndexed = 0;
            NumUrlsCrawled = 0;
            NumErrors = 0;
            IsLoading = false;
        }

        public string[] GetLastTen()
        {
            lock (lastTen)
            {
                return lastTen.ToArray();
            }
        }

        public void StopLoading()
        {
            stopLoading = true;
        }

        public bool CrawlNext()
        {
            if (stopLoading)
                throw new InvalidOperationException("Cannot crawl before resetting.");

            var urlMsg = urlsQueue.GetMessage(TimeSpan.FromMinutes(1));
            if (urlMsg == null)
                return false;

            workLimiter.WaitOne();
            ThreadPool.QueueUserWorkItem(ParsePageCallback, urlMsg);

            return true;
        }

        private void ParsePageCallback(object context)
        {
            try
            {
                ++NumUrlsCrawled;
                CloudQueueMessage msg = context as CloudQueueMessage;
                string pageUrl = msg.AsString;
                HtmlDocument doc = GetLoadedDoc(pageUrl);
                if (doc == null || stopLoading)
                    return;
                
                Trace.TraceInformation("Parsing '{0}'.", pageUrl);

                var batchOp = BuildIndexBatch(doc, pageUrl);
                if (batchOp != null)
                {
                    if (stopLoading)
                        return;

                    foreach (var op in batchOp)
                    {
                        indexTable.ExecuteAsync(op);
                    }
                    AddToLastTen(pageUrl);
                    NumUrlsIndexed++;
                }

                var links = doc.DocumentNode.SelectNodes("//a");
                if (links != null)
                {
                    foreach (var linkTag in links)
                    {
                        if (stopLoading)
                        {
                            return;
                        }
                        EnqueueUrlIfValid(linkTag.GetAttributeValue("href", ""),
                                          WebPageData.GetSimpleHost(pageUrl));
                    }
                }

                urlsQueue.DeleteMessageAsync(msg);
            }
            catch (Exception) { }
            finally
            {
                workLimiter.Release();
            }
        }

        private void ParseSitemap(string sitemap)
        {
            Queue<string> urls = new Queue<string>();
            urls.Enqueue(sitemap);

            while (urls.Count > 0 && !stopLoading)
            {
                string current = urls.Dequeue();
                using (Stream xmlStream = GetStreamFromUrl(current))
                {
                    if (xmlStream == null)
                        continue;

                    XmlDocument doc = new XmlDocument();
                    doc.Load(xmlStream);
                    if (doc.GetElementsByTagName("urlset").Count > 0)
                    {
                        foreach (XmlNode urlNode in doc.GetElementsByTagName("loc"))
                        {
                            if (stopLoading)
                                break;
                            EnqueueUrlIfValid(urlNode.InnerText);
                            Thread.Sleep(50);
                        }
                    }
                    else if (doc.GetElementsByTagName("sitemapindex").Count > 0)
                    {
                        string txt;
                        foreach (XmlNode urlNode in doc.GetElementsByTagName("loc"))
                        {
                            if (stopLoading)
                                break;
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

        private void EnqueueUrlIfValid(string url, string host)
        {
            //Uri hostUri;
            //Uri uri;
            //if (url.StartsWith("/") && !url.Contains(".com") &&
            //    Uri.TryCreate("http://" + host, UriKind.Absolute, out hostUri) &&
            //    Uri.TryCreate(hostUri, url, out uri))
            //{
            //    try
            //    {
            //        EnqueueUrlIfValid(uri.GetComponents(UriComponents.HttpRequestUrl, UriFormat.Unescaped));
            //    }
            //    catch (Exception e) { Trace.TraceError(e.ToString()); }
            //}
            //else
            EnqueueUrlIfValid(url);
        }

        private void EnqueueUrlIfValid(string url)
        {
            url = FixUrl(url);
            Uri uri;
            if (!Uri.TryCreate(url, UriKind.Absolute, out uri) || !IsValidUrl(uri))
                return;

            try
            {
                url = uri.GetComponents(UriComponents.HttpRequestUrl, UriFormat.Unescaped);
            }
            catch (Exception e)
            {
                Trace.TraceError(e.ToString());
                return; 
            }

            lock (urlsFound)
            {
                if (urlsFound.Contains(url))// && !UrlCrawled(uri))
                    return;
                urlsFound.Add(url);
            }

            CloudQueueMessage msg = new CloudQueueMessage(url);
            urlsQueue.BeginAddMessage(msg, (IAsyncResult ar) =>
            {
                Trace.TraceInformation("Thread {1} added to queue: {0}", url, 
                                        Thread.CurrentThread.ManagedThreadId);
            }, this);
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
            try
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
            catch (Exception e) { Trace.TraceError(e.ToString()); }

            return false;
        }

        private Stream GetStreamFromUrl(string url)
        {
            try
            {
                url = FixUrl(url);
                HttpWebRequest request = WebRequest.CreateHttp(url);
                request.Timeout = 10000;
                request.UserAgent = userAgent;
                return request.GetResponse().GetResponseStream();
            }
            catch (WebException webExp)
            {
                Trace.TraceError("{0} - {1}: {2}", url, webExp.Status, webExp.Message);
                ++NumErrors;
                
                CrawlerError error = new CrawlerError("crawler1", url);
                error.Status = webExp.Status.ToString();
                error.Message = webExp.Message;
                TableOperation insertOp = TableOperation.InsertOrMerge(error);
                errorTable.ExecuteAsync(insertOp);
            }
            catch (Exception e) { Trace.TraceError(e.ToString()); }
            
            return null;
        }

        private HtmlDocument GetLoadedDoc(string url)
        {
            try
            {
                using (Stream stream = GetStreamFromUrl(url))
                {
                    if (stream != null)
                    {
                        HtmlDocument doc = new HtmlDocument();
                        doc.Load(stream);
                        return doc;
                    }
                }
            }
            catch (Exception e)
            {
                Trace.TraceError("Failed to load html document for {0}, {1}", url, e.ToString());
            }
            return null;
        }

        private string FixUrl(string url)
        {
            if (url.StartsWith("http://") || url.StartsWith("https://"))
            {
                return url;
            }

            return "http://" + url;
        }

        private void AddToLastTen(string url)
        {
            lock (lastTen)
            {
                if (lastTen.Count >= 10)
                    lastTen.Dequeue();
                lastTen.Enqueue(url);
            }
        }

        private List<TableOperation> BuildIndexBatch(HtmlDocument doc, string pageUrl)
        {
            var titleNode = doc.DocumentNode.SelectSingleNode("//title");
            if (titleNode == null)
                titleNode = doc.DocumentNode.SelectSingleNode("//h1");
            if (titleNode == null || string.IsNullOrEmpty(titleNode.InnerText))
            {
                Trace.TraceInformation("Page title not found for '{0}'.", pageUrl);
                return null;
            }
            var date = doc.DocumentNode.SelectSingleNode("//meta[@itemprop='datePublished']");

            var title = titleNode.InnerText;
            var titleWords = Regex.Replace(title.Replace("'", ""), @"[^\w\s]", " ")
                .Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

            List<TableOperation> operation = new List<TableOperation>();
            WebPageData pageData = new WebPageData(pageUrl, title);
            if (date != null)
                pageData.DateCreated = date.GetAttributeValue("content", null);
            operation.Add(TableOperation.InsertOrReplace(pageData));

            HashSet<string> words = new HashSet<string>();
            foreach (var word in titleWords)
            {
                if (!words.Contains(word))
                {
                    int frequency = titleWords.Where(x => x == word).Count();
                    InvertedIndexEntry entry = new InvertedIndexEntry(word, pageUrl, title, 
                                                                      frequency);
                    operation.Add(TableOperation.InsertOrReplace(entry));
                    words.Add(word);
                }
            }

            return operation;
        }
    }
}
