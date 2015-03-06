using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CrawlerLibrary
{
    public enum CrawlerStatus
    {
        Paused = 0,
        Idle = 1,
        Loading = 2,
        Crawling = 3
    }

    public class CrawlerStateInfo : TableEntity
    {
        public CrawlerStatus Status 
        {
            get
            {
                return (CrawlerStatus)CrawlerStatus;
            }
            set
            {
                CrawlerStatus = (int)value;
            }
        }

        public int CrawlerStatus { get; set; }
        public int UrlsIndexed { get; set; }
        public int UrlsCrawled { get; set; }
        public string LastTenUrls { get; set; }
        public int ErrorCount { get; set; }
        public int CpuUtilization { get; set; }
        public long RamUsed { get; set; }
        public long RamAvailable { get; set; }

        public CrawlerStateInfo(string name, CrawlerStatus status)
        {
            this.PartitionKey = "CrawlerStateInfo";
            this.RowKey = name;
            this.Status = status;
        }

        public CrawlerStateInfo() { }

    }
}
