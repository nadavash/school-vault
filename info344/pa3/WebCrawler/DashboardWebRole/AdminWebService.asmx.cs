using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Queue;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;
using System.Web.Script.Services;
using System.Web.Services;
using CrawlerLibrary;
using System.Threading.Tasks;
using Microsoft.WindowsAzure.ServiceRuntime;
using System.Net;
using System.Diagnostics;

namespace DashboardWebRole
{
    /// <summary>
    /// Summary description for AdminWebService
    /// </summary>
    [WebService(Namespace = "http://tempuri.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    // To allow this Web Service to be called from script, using ASP.NET AJAX, uncomment the following line. 
    [System.Web.Script.Services.ScriptService]
    public class AdminWebService : System.Web.Services.WebService
    {
        private CloudStorageAccount storageAccount;

        public AdminWebService()
        {
            storageAccount = CloudStorageAccount.Parse(
                RoleEnvironment.GetConfigurationSettingValue("StorageConnectionString"));
        }

        [WebMethod]
        public string GetTitleFromIndex(string url)
        {
            Uri uri = FixUrl(url);
            if (uri == null)
                return null;

            string host = WebPageData.GetSimpleHost(url);
            string encoded = WebUtility.UrlEncode(
                uri.GetComponents(UriComponents.HttpRequestUrl, UriFormat.Unescaped));
            TableQuery<WebPageData> query = new TableQuery<WebPageData>()
                .Where(TableQuery.CombineFilters(
                    TableQuery.GenerateFilterCondition("PartitionKey",
                                                       QueryComparisons.Equal, host),
                    "and",
                    TableQuery.GenerateFilterCondition("RowKey",
                                                       QueryComparisons.Equal, encoded)));
            
            try
            {
                var result = GetTable("crawlerindex").ExecuteQuery(query);
                if (result.Count() < 1)
                    return null;
                else
                    return result.First().Title;
            }
            catch (Exception)
            {
                return null;
            }
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public CrawlerError[] GetErrors()
        {
            DateTime last = DateTime.UtcNow - TimeSpan.FromHours(1);
            string queryStr = String.Format("PartitionKey eq 'CrawlerError' and Timestamp ge datetime'{0}'", 
                last.ToString("s"));
            var query = new TableQuery<CrawlerError>().Where(queryStr);

            try
            {
                var result = GetTable("workerstatus").ExecuteQuery(query);
                int count = Math.Min(result.Count(), 20);
                if (count == 0)
                    return null;
                return result.Take(count).ToArray();
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.ToString());
                return null;
            }
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string StartCrawler()
        {
            try
            {
                CloudQueue commands = GetQueue("commandqueue");
                CloudQueueMessage msg = new CloudQueueMessage("crawl");
                commands.AddMessage(msg);
            }
            catch (Exception e)
            {
                return e.ToString();
            }

            return "Sent start crawler message.";
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string StopCrawler()
        {
            try
            {
                CloudQueue commands = GetQueue("commandqueue");
                CloudQueueMessage msg = new CloudQueueMessage("pause");
                commands.AddMessage(msg, TimeSpan.FromSeconds(10));
            }
            catch (Exception e)
            {
                return e.ToString();
            }

            return "Sent stop crawler message.";
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string ClearUrlQueue()
        {
            try
            {
                GetQueue("urlqueue").Clear();
            }
            catch (Exception e)
            {
                return e.ToString();
            }

            return "Url queue cleared.";
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string DeleteIndex()
        {
            return DeleteTable("crawlerindex", "Index deleted.");
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string DeleteErrors()
        {
            return DeleteTable("workerstatus", "Errors deleted.");
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true, ResponseFormat = ResponseFormat.Json)]
        public DashboardData GetDashboardData()
        {
            try
            {
                return BuildDashboardData();
            }
            catch (Exception) { }

            return null;
        }

        private DashboardData BuildDashboardData()
        {
            DashboardData data = new DashboardData();
            CloudQueue urlsQueue = GetQueue("urlqueue");
            urlsQueue.FetchAttributes();
            int? msgCount = urlsQueue.ApproximateMessageCount;
            var msgs = urlsQueue.PeekMessages(10);

            data.NumQueuedUrls = msgCount != null ? msgCount.Value : 0;
            data.QueuedUrls =  msgs.Select(x => x.AsString).ToArray();

            try { GetTable("crawlerindex"); }
            catch (Exception) { }

            CloudTable crawlerStates = GetTable("workerstatus");
            TableQuery<CrawlerStateInfo> states = new TableQuery<CrawlerStateInfo>();
            states.Where(TableQuery.GenerateFilterCondition("PartitionKey", 
                                                            QueryComparisons.Equal, 
                                                            "CrawlerStateInfo"));
            data.CrawlerStates = crawlerStates.ExecuteQuery(states).ToArray();

            return data;
        }

        private CloudQueue GetQueue(string qref)
        {
            CloudQueueClient queueClient = storageAccount.CreateCloudQueueClient();
            CloudQueue queue = queueClient.GetQueueReference(qref);
            queue.CreateIfNotExists();
            return queue;
        }

        private CloudTable GetTable(string tableref)
        {
            CloudTableClient tableClient = storageAccount.CreateCloudTableClient();
            CloudTable table = tableClient.GetTableReference(tableref);
            table.CreateIfNotExists();
            return table;
        }

        private string DeleteTable(string tableName, string msg)
        {
            try
            {
                GetTable(tableName).DeleteIfExists();
            }
            catch (Exception e)
            {
                return e.ToString();
            }

            return msg;
        }

        private Uri FixUrl(string url)
        {
            if (url == null)
                return null;

            if (!url.StartsWith("http:") && !url.StartsWith("https:"))
                url = "http://" + url; 
            Uri uri;
            if (!Uri.TryCreate(url, UriKind.Absolute, out uri))
                return null;

            return uri;
        }

        public class DashboardData
        {
            public int NumQueuedUrls { get; set; }
            public string[] QueuedUrls { get; set; }
            public CrawlerStateInfo[] CrawlerStates { get; set; }
        }
    }
}
