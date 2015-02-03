using System;
using System.Collections.Generic;
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
            using (StreamReader reader = new StreamReader())
            {
                prefixTree = new Trie();
                prefixTree.ParseFromFile(reader);
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
            string[] suggestions = new string[max];
            for (int i = 0; i < max; ++i)
            {
                suggestions[i] = prefix;
            }
            return suggestions;
        }
    }
}
