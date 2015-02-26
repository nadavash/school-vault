using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace CrawlerLibrary
{
    public class WebPageData : TableEntity
    {
        public static string GetSimpleHost(string url)
        {
            Uri uri = new Uri(url);
            return uri.GetComponents(UriComponents.Host, UriFormat.Unescaped)
                      .Replace("www.", "");
        }

        public string Url
        {
            get
            {
                return WebUtility.UrlDecode(RowKey);
            }
        }
        public string Title { get; set; }
        public string DateCreated { get; set; }

        public WebPageData(string url, string title)
        {
            this.PartitionKey = GetSimpleHost(url);
            this.RowKey = WebUtility.UrlEncode(url);
            this.Title = title;
        }

        public WebPageData() { }
    }
}
