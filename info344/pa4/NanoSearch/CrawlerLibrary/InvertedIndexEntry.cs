using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace CrawlerLibrary
{
    public class InvertedIndexEntry : TableEntity
    {
        public string Word
        {
            get
            {
                return this.PartitionKey;
            }
        }
        public string Url
        {
            get
            {
                return WebUtility.UrlDecode(RowKey);
            }
        }
        public string Title { get; set; }
        public int Frequency { get; set; }
        public string DateCreated { get; set; }

        public InvertedIndexEntry(string word, string url, string title, int frequency)
        {
            word = word.ToLower();
            this.PartitionKey = word;
            this.RowKey = WebUtility.UrlEncode(url);
            this.Title = title;
            this.Frequency = frequency;
        }

        public InvertedIndexEntry() { }
    }
}
