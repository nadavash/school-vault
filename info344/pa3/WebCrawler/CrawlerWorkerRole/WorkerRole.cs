using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.Diagnostics;
using Microsoft.WindowsAzure.ServiceRuntime;
using Microsoft.WindowsAzure.Storage;
using System.Configuration;
using Microsoft.WindowsAzure.Storage.Queue;
using Microsoft.WindowsAzure.Storage.Table;
using CrawlerLibrary;

namespace CrawlerWorkerRole
{
    public class WorkerRole : RoleEntryPoint
    {
        public const int SleepTimeMillis = 25;
        private readonly CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
        private readonly ManualResetEvent runCompleteEvent = new ManualResetEvent(false);

        private CloudStorageAccount storageAccount;

        private CloudTable indexTable;
        private CloudTable statusTable;

        private CloudQueue commandsQueue;
        private CloudQueue urlsQueue;

        public override void Run()
        {
            Trace.TraceInformation("CrawlerWorkerRole is running");
            InitAzureStorage();

            WebCrawler crawler = new WebCrawler("MyLittleCrawler", urlsQueue, indexTable);
            crawler.InitializeCrawler("www.bleacherreport.com", "www.cnn.com");

            string command = null;
            while (command != "kill")
            {
                CloudQueueMessage msg = commandsQueue.GetMessage();
                if (msg != null)
                {
                    commandsQueue.DeleteMessageAsync(msg);
                 
                    // Process new command if different.
                    if (command != msg.AsString) 
                    {
                        command = msg.AsString;
                        ProcessNewCommand(command, crawler);
                    }
                }
                else if (command != null)
                    ProcessCurrent(command, crawler);

                Thread.Sleep(SleepTimeMillis);
            }
        }

        public override bool OnStart()
        {
            // Set the maximum number of concurrent connections
            ServicePointManager.DefaultConnectionLimit = 12;

            // For information on handling configuration changes
            // see the MSDN topic at http://go.microsoft.com/fwlink/?LinkId=166357.

            bool result = base.OnStart();

            Trace.TraceInformation("CrawlerWorkerRole has been started");

            return result;
        }

        public override void OnStop()
        {
            Trace.TraceInformation("CrawlerWorkerRole is stopping");

            this.cancellationTokenSource.Cancel();
            this.runCompleteEvent.WaitOne();

            base.OnStop();

            Trace.TraceInformation("CrawlerWorkerRole has stopped");
        }

        private void ProcessNewCommand(string command, WebCrawler crawler)
        {
            Trace.TraceInformation("Received command '{0}' from commands queue.", command);
            switch (command)
            {
                case "crawl":
                    crawler.Reset();
                    crawler.CrawlSitemaps();
                    break;
                case "pause":
                    crawler.StopLoading();
                    break;
            }
        }

        private void ProcessCurrent(string lastCommand, WebCrawler crawler)
        {
            bool idle = false;
            switch (lastCommand)
            {
                case "crawl":
                    idle = crawler.CrawlNext();
                    break;
                case "pause":
                    idle = true;
                    break;
            }
        }

        private void InitAzureStorage()
        {
            storageAccount = CloudStorageAccount.Parse(
                ConfigurationManager.ConnectionStrings["CrawlerWorkerRole.Properties.Settings.StorageConnectionString"]
                .ConnectionString);
            CloudQueueClient queueClient = storageAccount.CreateCloudQueueClient();
            commandsQueue = queueClient.GetQueueReference("commandqueue");
            commandsQueue.CreateIfNotExists();
            urlsQueue = queueClient.GetQueueReference("urlqueue");
            urlsQueue.CreateIfNotExists();

            CloudTableClient tableClient = storageAccount.CreateCloudTableClient();
            indexTable = tableClient.GetTableReference("crawlerindex");
            indexTable.CreateIfNotExists();
            statusTable = tableClient.GetTableReference("workerstatus");
            statusTable.CreateIfNotExists();
        }
    }
}
