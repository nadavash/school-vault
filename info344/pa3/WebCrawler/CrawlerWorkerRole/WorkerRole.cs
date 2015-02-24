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

namespace CrawlerWorkerRole
{
    public class WorkerRole : RoleEntryPoint
    {
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
            //InitAzureStorage();

            try
            {
                this.RunAsync(this.cancellationTokenSource.Token).Wait();
            }
            finally
            {
                this.runCompleteEvent.Set();
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

        private async Task RunAsync(CancellationToken cancellationToken)
        {
            // TODO: Replace the following with your own logic.
            while (!cancellationToken.IsCancellationRequested)
            {
                Trace.TraceInformation("Working");




                await Task.Delay(1000);
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
