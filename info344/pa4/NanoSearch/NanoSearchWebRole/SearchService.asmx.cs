using CrawlerLibrary;
using Microsoft.WindowsAzure.ServiceRuntime;
using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using System.Web;
using System.Web.Caching;
using System.Web.Script.Services;
using System.Web.Services;

namespace NanoSearch
{
    /// <summary>
    /// Summary description for SearchService
    /// </summary>
    [WebService(Namespace = "http://tempuri.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    // To allow this Web Service to be called from script, using ASP.NET AJAX, uncomment the following line. 
    [System.Web.Script.Services.ScriptService]
    public class SearchService : System.Web.Services.WebService
    {
        private static Cache cache = HttpRuntime.Cache;

        private CloudStorageAccount storageAccount;

        public SearchService()
        {
            storageAccount = CloudStorageAccount.Parse(
                RoleEnvironment.GetConfigurationSettingValue("StorageConnectionString"));
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true, ResponseFormat = ResponseFormat.Json)]
        public SearchResult[] GetSearchResults(string q)
        {
            if (string.IsNullOrEmpty(q))
                return null;
            // Cleanup punctuation and lowercase the query.
            q = Regex.Replace(q, @"[^\w\s]", "");
            q = q.ToLower();

            CloudTableClient tableClient = storageAccount.CreateCloudTableClient();
            CloudTable table = tableClient.GetTableReference("crawlerindex");
            table.CreateIfNotExists();

            SearchResult[] results = cache[q] as SearchResult[];
            if (results != null)
                return results;

            TableQuery<InvertedIndexEntry> tableQuery = GenerateQuery(q);
            if (tableQuery == null)
                return null;

            results = table.ExecuteQuery(tableQuery)
                .GroupBy(x => x.Url)
                .Select(x => new SearchResult(x.Key, x.ElementAt(0).Title, x.Count()))
                .OrderByDescending(x => x.Rank)
                .ThenBy(x => x.Title)
                .ToArray();

            cache.Insert(q, results, null, Cache.NoAbsoluteExpiration, TimeSpan.FromMinutes(20));

            return results;
        }

        private TableQuery<InvertedIndexEntry> GenerateQuery(string queryString)
        {
            var filters = queryString.Split()
                .Select(word => TableQuery.GenerateFilterCondition("PartitionKey",
                                                                   QueryComparisons.Equal,
                                                                   word))
                .ToArray();

            string tableQueryString = null;
            if (filters.Length == 0)
                return null;

            tableQueryString = filters[0];
            
            foreach (var filter in filters.Skip(1))
            {
                tableQueryString = TableQuery.CombineFilters(tableQueryString, TableOperators.Or, filter);
            }

            return new TableQuery<InvertedIndexEntry>().Where(tableQueryString);
        }

        public class SearchResult
        {
            public string Url { get; set; }
            public string Title { get; set; }
            public int Rank { get; set; }

            public SearchResult() { }

            public SearchResult(string url, string title, int rank)
            {
                Url = url;
                Title = title;
                Rank = rank;
            }
        }
    }
}
