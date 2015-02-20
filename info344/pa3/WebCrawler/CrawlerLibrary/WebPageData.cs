using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CrawlerLibrary
{
    class WebPageData : TableEntity
    {
        public string URL { get; set; }
        public string Title { get; set; }

        public WebPageData() { }
    }
}
