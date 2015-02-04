using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Blob;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.Script.Services;
using System.Web.Services;

namespace LiveQuerySuggestions
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
        private static Trie prefixTree;

        static QuerySuggestionsService()
        {
            using (StreamReader wikiStream = LoadEntries())
            {
                prefixTree = new Trie();
                prefixTree.CreateFromStream(wikiStream);
            }
        }

        private static StreamReader LoadEntries()
        {
            string filePath = Environment.GetFolderPath(
                Environment.SpecialFolder.ApplicationData).ToString()
                + "\\wiki.txt";
            Debug.WriteLine(filePath);
            Stream stream = null;
            if (!File.Exists(filePath))
            {
                CloudStorageAccount storage = CloudStorageAccount.Parse(
                    ConfigurationManager.AppSettings["StorageConnectionString"]);
                CloudBlobClient blobClient = storage.CreateCloudBlobClient();
                CloudBlobContainer container = blobClient.GetContainerReference("live-suggestions");

                if (container.Exists())
                {
                    foreach (IListBlobItem item in container.ListBlobs(null, false))
                    {
                        if (item is CloudBlockBlob)
                        {
                            CloudBlockBlob blob = item as CloudBlockBlob;
                            stream = new FileStream(filePath, FileMode.Create);
                            blob.DownloadToStream(stream);
                            stream.Seek(0, SeekOrigin.Begin);
                        }
                    }
                }
            }
            else
            {
                stream = new FileStream(filePath, FileMode.Open);
            }

            return new StreamReader(stream);
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
            prefix = prefix.Trim();
            if (prefix == string.Empty)
                return null;
            return prefixTree.PrefixSearch(prefix).ToArray();
        }
    }
}
