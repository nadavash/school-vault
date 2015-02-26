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
        public const int SleepTimeMillis = 80;

        private readonly CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
        private readonly ManualResetEvent runCompleteEvent = new ManualResetEvent(false);
        
        // Perf counters
        private PerformanceCounter cpuUsage;
        private PerformanceCounter ramUsage;
        private PerformanceCounter ramAvailable;
        private int cpuUsagePercentage;

        // Cloud storage
        private CloudStorageAccount storageAccount;

        private CloudTable indexTable;
        private CloudTable statusTable;

        private CloudQueue commandsQueue;
        private CloudQueue urlsQueue;

        public override void Run()
        {
            Trace.TraceInformation("CrawlerWorkerRole is running");
            InitAzureStorage();
            CancellationToken token = cancellationTokenSource.Token;

            cpuUsage.NextValue();
            WebCrawler crawler = new WebCrawler("MyLittleCrawler", urlsQueue, 
                                                indexTable, statusTable);
            crawler.InitializeCrawler("www.bleacherreport.com", "www.cnn.com");
            string command = null;
            CrawlerStatus status = CrawlerStatus.Paused;
            int counter = 0;
            while (command != "kill" && !token.IsCancellationRequested)
            {
                CloudQueueMessage msg = GetNextMessage();
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
                {
                    status = ProcessCurrent(command, crawler);
                }

                Thread.Sleep(SleepTimeMillis);

                if (counter >= 15)
                {
                    cpuUsagePercentage = (int)cpuUsage.NextValue();
                    counter = 0;
                    CrawlerStateInfo info = BuildCrawlerStateInfo(crawler, status);
                    PostCrawlerStateInfo(info);
                }

                ++counter;
            }

            runCompleteEvent.Reset();
        }

        public override bool OnStart()
        {
            // Set the maximum number of concurrent connections
            ServicePointManager.DefaultConnectionLimit = 12;

            // For information on handling configuration changes
            // see the MSDN topic at http://go.microsoft.com/fwlink/?LinkId=166357.

            bool result = base.OnStart();

            Trace.TraceInformation("CrawlerWorkerRole has been started");

            cpuUsage = new PerformanceCounter("Process", "% Processor Time", 
                                              Process.GetCurrentProcess().ProcessName);
            ramUsage = new PerformanceCounter("Memory", "% Committed Bytes In Use");
            ramAvailable = new PerformanceCounter("Memory", "Available MBytes");

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

        private CrawlerStatus ProcessCurrent(string lastCommand, WebCrawler crawler)
        {
            CrawlerStatus status = CrawlerStatus.Idle;
            switch (lastCommand)
            {
                case "crawl":
                    if (crawler.IsLoading)
                    {
                        status = CrawlerStatus.Loading;
                        crawler.CrawlNext();
                    }
                    else if (crawler.CrawlNext())
                        status = CrawlerStatus.Crawling;
                    break;
                case "pause":
                    status = CrawlerStatus.Paused;
                    break;
            }

            return status;
        }

        private CrawlerStateInfo BuildCrawlerStateInfo(WebCrawler crawler, CrawlerStatus status)
        {
            CrawlerStateInfo stateInfo = new CrawlerStateInfo("crawler1", status);
            stateInfo.ErrorCount = crawler.NumErrors;
            stateInfo.LastTenUrls = String.Join(",", crawler.GetLastTen());
            stateInfo.UrlsCrawled = crawler.NumUrlsCrawled;
            stateInfo.UrlsIndexed = crawler.NumUrlsIndexed;
            stateInfo.CpuUtilization = cpuUsagePercentage;
            stateInfo.RamAvailable = (long)ramAvailable.NextValue();
            stateInfo.RamUsed = (long)ramUsage.NextValue();

            return stateInfo;
        }

        private void PostCrawlerStateInfo(CrawlerStateInfo info)
        {
            TableOperation insertOp = TableOperation.InsertOrMerge(info);
            statusTable.ExecuteAsync(insertOp);
        }

        private void InitAzureStorage()
        {
            storageAccount = CloudStorageAccount.Parse(
                RoleEnvironment.GetConfigurationSettingValue("StorageConnectionString"));
            CloudQueueClient queueClient = storageAccount.CreateCloudQueueClient();
            commandsQueue = queueClient.GetQueueReference("commandqueue");
            commandsQueue.CreateIfNotExists();
            urlsQueue = queueClient.GetQueueReference("urlqueue");
            urlsQueue.CreateIfNotExists();

            CloudTableClient tableClient = storageAccount.CreateCloudTableClient();
            while (true)
            {
                try
                {
                    indexTable = tableClient.GetTableReference("crawlerindex");
                    indexTable.CreateIfNotExists();
                    statusTable = tableClient.GetTableReference("workerstatus");
                    statusTable.CreateIfNotExists();
                    Thread.Sleep(1000);
                    break;
                }
                catch (Exception e) { }
            }
        }

        private CloudQueueMessage GetNextMessage()
        {
            try
            {
                CloudQueueMessage msg = commandsQueue.GetMessage();
                return msg;
            }
            catch (Exception e)
            {
                Trace.TraceError(e.ToString());
                return null;
            }
        }
    }
}
