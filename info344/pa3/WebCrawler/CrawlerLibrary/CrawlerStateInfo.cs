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
        Idle,
        Loading,
        Crawling
    }

    class CrawlerStateInfo : TableEntity
    {
        public CrawlerStatus Status { get; set; }
        public int EnqueuedUrls { get; set; }
        public int ErrorCount { get; set; }
        public short CpuUtilization { get; set; }
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
