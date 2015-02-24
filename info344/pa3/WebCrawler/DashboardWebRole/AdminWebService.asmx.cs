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
                ConfigurationManager.ConnectionStrings["DashboardWebRole.Properties.Settings.StorageConnectionString"]
                .ConnectionString);
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
                commands.AddMessage(msg);
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
        public int IndexSize()
        {
            CloudTable index = GetTable("crawlerindex");
            return 0;
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
    }
}
