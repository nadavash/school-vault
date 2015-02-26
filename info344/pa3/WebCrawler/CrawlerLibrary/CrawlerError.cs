using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace CrawlerLibrary
{
    public class CrawlerError : TableEntity
    {
        public string Url 
        {
            get { return WebUtility.UrlDecode(this.RowKey); } 
        }
        public string CrawlerName { get; set; }
        public string Status { get; set; }
        public string Message { get; set; }

        public CrawlerError(string crawlerName, string url)
        {
            this.PartitionKey = "CrawlerError";
            this.RowKey = WebUtility.UrlEncode(url);
        }

        public CrawlerError() { }

    }
}
