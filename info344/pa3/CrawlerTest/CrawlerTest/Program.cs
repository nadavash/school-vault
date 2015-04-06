using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CrawlerLibrary;
using System.Net;
using System.IO;
using Microsoft.WindowsAzure.Storage;
using System.Configuration;
using Microsoft.WindowsAzure.Storage.Table;
using Microsoft.WindowsAzure.Storage.Queue;
using System.Threading;

namespace CrawlerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            RobotsParser parser = new RobotsParser("MyLittleCrawler");
            WebRequest request = WebRequest.CreateHttp("http://www.bleacherreport.com/robots.txt");
            WebResponse response = request.GetResponse();
            parser.Parse(new StreamReader(response.GetResponseStream()));
            Console.WriteLine(parser.IsUrlAllowed("http://www.bleacherreport.com/users/rankings"));

            CloudStorageAccount acc = CloudStorageAccount.Parse(
                ConfigurationManager.AppSettings["StorageConnectionString"]);
            CloudTableClient tableClient = acc.CreateCloudTableClient();
            CloudTable table = tableClient.GetTableReference("crawlerindex");
            table.CreateIfNotExists();
            CloudQueueClient queueClient = acc.CreateCloudQueueClient();
            CloudQueue queue = queueClient.GetQueueReference("urlqueue");
            queue.CreateIfNotExists();

            string address = "http://www.bleacherreport.com/index.html?hello=33#abc";
            Uri uri = new Uri(address);
            Console.WriteLine(uri.GetComponents(UriComponents.HttpRequestUrl, UriFormat.Unescaped));
            Console.WriteLine(uri.GetComponents(UriComponents.Path, UriFormat.Unescaped));
            int th;
            int yo;
            ThreadPool.GetAvailableThreads(out th, out yo);
            Console.WriteLine(th + " " + yo);
            Console.WriteLine(WebUtility.UrlEncode(address));

            WebCrawler crawler = new WebCrawler("MyLittleCrawler", queue, table);
            crawler.Clear();
            crawler.InitializeCrawler("www.bleacherreport.com", "www.cnn.com");
            crawler.CrawlSitemaps();

            while (true)
            {
                crawler.CrawlNext();
                Thread.Sleep(50);
            }

            Console.ReadKey();

        }
    }
}
