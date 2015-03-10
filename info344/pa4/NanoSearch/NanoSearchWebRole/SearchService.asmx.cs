using CrawlerLibrary;
using Microsoft.WindowsAzure.ServiceRuntime;
using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.Collections.Generic;
using System.Diagnostics;
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
            Trace.TraceInformation("Getting search results for '{0}'", q);
            // Cleanup punctuation and lowercase the query.
            q = Regex.Replace(q.Replace("'", ""), @"[^\w\s]", " ");
            q = q.ToLower();

            CloudTableClient tableClient = storageAccount.CreateCloudTableClient();
            CloudTable table = tableClient.GetTableReference("crawlerindex");
            table.CreateIfNotExists();

            SearchResult[] results = cache[q] as SearchResult[];
            if (results != null)
                return results;

            var queries = GenerateQueries(q);
            if (queries == null)
                return null;

            results = ExecuteQueries(table, queries)
                .GroupBy(x => x.Url)
                .Select(x => new SearchResult(x.Key, x.ElementAt(0).Title, 
                                              x.Select(y => y.Frequency).Sum() *
                                              x.Count()))
                .GroupBy(x => x.Title).Select(x => x.FirstOrDefault())
                .OrderByDescending(x => x.Rank)
                .ThenBy(x => x.Title)
                .Take(10)
                .ToArray();

            cache.Insert(q, results, null, Cache.NoAbsoluteExpiration, TimeSpan.FromHours(1));

            return results;
        }

        private TableQuery<InvertedIndexEntry>[] GenerateQueries(string queryString)
        {
            var filters = queryString.Split()
                .Select(word => TableQuery.GenerateFilterCondition("PartitionKey",
                                                                   QueryComparisons.Equal,
                                                                   word))
                .ToArray();

            if (filters.Length == 0)
                return null;

            var queries = new TableQuery<InvertedIndexEntry>[filters.Length];

            for (int i = 0; i < queries.Length; ++i)
            {
                queries[i] = new TableQuery<InvertedIndexEntry>().Where(filters[i]);
            }

            return queries;
        }

        private IEnumerable<InvertedIndexEntry> ExecuteQueries(CloudTable table,
                                                               TableQuery<InvertedIndexEntry>[] queries)
        {
            List<InvertedIndexEntry> results = new List<InvertedIndexEntry>();
            foreach (var query in queries)
            {
                results.AddRange(table.ExecuteQuery(query));
            }

            return results;
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
