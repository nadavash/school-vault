using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Queue;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;
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
        public string HelloWorld()
        {
            return "Hello World";
        }

        public string StartCrawler()
        {
            try
            {
                CloudQueue commands = GetQueue("commandqueue");
                CloudQueueMessage msg = new CloudQueueMessage("start");
                commands.AddMessage(msg);
            }
            catch (Exception e)
            {
                return e.ToString();
            }

            return "Sent start crawler message.";
        }

        public string StopCrawler()
        {
            try
            {
                CloudQueue commands = GetQueue("commandqueue");
                CloudQueueMessage msg = new CloudQueueMessage("stop");
                commands.AddMessage(msg);
            }
            catch (Exception e)
            {
                return e.ToString();
            }

            return "Sent stop crawler message.";
        }

        public int IndexSize()
        {
            CloudTable index = GetTable("crawlerindex");
            return 0;
        }

        public CloudQueue GetQueue(string qref)
        {
            CloudQueueClient queueClient = storageAccount.CreateCloudQueueClient();
            CloudQueue queue = queueClient.GetQueueReference(qref);
            queue.CreateIfNotExists();
            return queue;
        }

        public CloudTable GetTable(string tableref)
        {
            CloudTableClient tableClient = storageAccount.CreateCloudTableClient();
            CloudTable table = tableClient.GetTableReference(tableref);
            table.CreateIfNotExists();
            return table;
        }
    }
}
