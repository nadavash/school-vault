using Microsoft.WindowsAzure.ServiceRuntime;
using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Blob;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.IO;
using System.Timers;
using System.Web;
using System.Web.Script.Services;
using System.Web.Services;

namespace NanoSearch
{
    /// <summary>
    /// Summary description for QuerySuggestions
    /// </summary>
    [WebService(Namespace = "http://tempuri.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    // To allow this Web Service to be called from script, using ASP.NET AJAX, uncomment the following line. 
    [System.Web.Script.Services.ScriptService]
    public class QuerySuggestionsService : System.Web.Services.WebService
    {
        public const string WIKI_FILE = "\\wiki.txt";
        public const string PAGECOUNTS_FILE = "\\pagecounts.txt";
        private static Trie prefixTree;
        private static Dictionary<string, int> pageCounts;

        static QuerySuggestionsService()
        {
            string folderPath = RoleEnvironment.GetLocalResource("LocalStorage").RootPath;
                //Environment.GetFolderPath(
                //Environment.SpecialFolder.ApplicationData).ToString();

            if (!File.Exists(folderPath + WIKI_FILE)
                || !File.Exists(folderPath + PAGECOUNTS_FILE))
                DownloadFromBlob(folderPath);

            using (StreamReader wikis = new StreamReader(folderPath + WIKI_FILE))
            {
                Debug.WriteLine("Populating trie from stream.");
                prefixTree = new Trie();
                prefixTree.PopulateFromStream(wikis);
            }

            using (StreamReader counts = new StreamReader(folderPath + PAGECOUNTS_FILE))
            {
                Debug.WriteLine("Populating page counts.");
                pageCounts = new Dictionary<string, int>();
                string line = null;
                while ((line = counts.ReadLine()) != null)
                {
                    string[] split = line.Split(':');
                    pageCounts[split[0]] = int.Parse(split[1]);
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="prefix"></param>
        /// <param name="max"></param>
        /// <returns></returns>
        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string[] GetSuggestions(string prefix, int max)
        {
            prefix = prefix.Trim().ToLower();
            if (prefix == string.Empty)
                return null;
            max = Math.Min(10, Math.Max(5, max));

            Debug.WriteLine("Trying prefix search.");
            List<string> matches = prefixTree.PrefixSearch(prefix, max);
            if (matches.Count == 0)
            {
                Debug.WriteLine("Trying fuzzy search...");
                matches = prefixTree.FuzzyPrefixSearch(prefix, max);
                for (int i = 0; i < matches.Count; ++i)
                {
                    matches[i] = "<b>" + matches[i] + "</b>?";
                }
            }

            return matches.ToArray();
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string[][] GetSuggestionsRanked(string prefix, int max)
        {
            prefix = prefix.Trim().ToLower();
            if (prefix == string.Empty)
                return null;
            max = Math.Min(10, Math.Max(5, max));

            Debug.WriteLine("Trying prefix search.");
            List<string> matches = prefixTree.PrefixSearch(prefix, max);

            int fuzzyCount = 0;
            if (matches.Count < 10)
            {
                Debug.WriteLine("Trying fuzzy search...");
                var fuzzyMatches = prefixTree.FuzzyPrefixSearch(prefix, max);

                while (matches.Count < 10 && fuzzyMatches.Count > 0)
                {
                    if (!matches.Contains(fuzzyMatches[0]))
                    {
                        matches.Add(fuzzyMatches[0]);
                        fuzzyCount++;
                    }
                    fuzzyMatches.RemoveAt(0);
                }
            }

            var ranked = RankSuggestions(matches, fuzzyCount);
            string[][] results = new string[ranked.Length][];
            for (int i = 0; i < ranked.Length; ++i)
            {
                results[i] = new string[2];
                results[i][0] = ranked[i].Key;
                results[i][1] = ranked[i].Value.ToString();
            }

            return results;
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public int NumUrls()
        {
            return prefixTree.Count;
        }

        [WebMethod]
        [ScriptMethod(UseHttpGet = true)]
        public string LastTitle()
        {
            return prefixTree.LastTitle;
        }

        private KeyValuePair<string, int>[] RankSuggestions(List<string> suggestions, int numFuzzy)
        {
            var sorted = new List<KeyValuePair<string,int>>();
            int index = 0;
            foreach (var sug in suggestions)
            {
                var actual = index < suggestions.Count - numFuzzy ? sug : "<em>" + sug + "</em>";
                if (pageCounts.ContainsKey(sug))
                {
                    int currVal = pageCounts[sug];
                    int i = 0;
                    foreach (var keyVal in sorted)
                    {
                        if (keyVal.Value < currVal)
                            break;
                        ++i;
                    }
                    sorted.Insert(i, new KeyValuePair<string, int>(actual, currVal));
                }
                else
                {
                    sorted.Add(new KeyValuePair<string, int>(actual, 0));
                }

                ++index;
            }

            return sorted.ToArray();
        }

        private static void DownloadFromBlob(string folderPath)
        {
            CloudStorageAccount storage = CloudStorageAccount.Parse(
                RoleEnvironment.GetConfigurationSettingValue("StorageConnectionString"));
            CloudBlobClient blobClient = storage.CreateCloudBlobClient();
            CloudBlobContainer container = blobClient.GetContainerReference("live-suggestions");
            Stream stream = null;
            if (container.Exists())
            {
                foreach (IListBlobItem item in container.ListBlobs(null, false))
                {
                    if (item is CloudBlockBlob)
                    {
                        Debug.WriteLine("Streaming blob to file.");
                        CloudBlockBlob blob = item as CloudBlockBlob;
                        using (stream = new FileStream(folderPath + "\\" + blob.Name, FileMode.Create))
                        {
                            blob.DownloadToStream(stream);
                        }                        
                    }
                }
            }
        }
    }
}
