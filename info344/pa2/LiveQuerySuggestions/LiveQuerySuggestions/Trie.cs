using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO;
using System.Text;
using System.Web;

namespace LiveQuerySuggestions
{
    /// <summary>
    /// 
    /// </summary>
    public class Trie
    {
        public const int MAX_NODE_ENTRIES = 100;
        public const int MAX_DIST = 2;
        public const int MAX_LEN_DIFF = 5;

        private TrieNode root;
        private HashSet<string> set;

        public Trie()
        {
            root = new TrieNode();
            set = new HashSet<string>();
        }

        public void PopulateFromStream(StreamReader reader)
        {
            string word;
            while ((word = reader.ReadLine()) != null)
            {
                Add(word);
            }
        }

        public void Add(string word)
        {
            AddFrom(root, word);
        }

        private void AddFrom(TrieNode source, string word)
        {
            if (word == string.Empty)
                return;

            TrieNode current = source;
            int index = 0;
            foreach (char c in word)
            {
                if (current.IsBasic)
                {
                    current.Entries.Add(word.Substring(index));

                    if (current.Entries.Count > MAX_NODE_ENTRIES)
                    {
                        SortedSet<string> entries = current.ConvertToFullTrieNode();
                        foreach (var s in entries)
                        {
                            AddFrom(current, s);
                        }
                    }

                    return;
                }
                else if (!current.Children.HasChild(c))
                {
                    current.Children[c] = new TrieNode();
                }
                current = current.Children[c];

                ++index;
            }

            current.IsFullEntry = true;
        }

        public List<string> PrefixSearch(string prefix, int max)
        {
            return PrefixSearch(prefix, max, false);
        }

        private void FindEntriesFromNode(string actual, TrieNode node,
            StringBuilder prefix, int max, List<string> entries)
        {
            if (node == null)
                return;

            string strPrefix = prefix.ToString();
            if (node.IsFullEntry && strPrefix.StartsWith(actual))
                entries.Add(strPrefix);

            if (node.IsBasic)
            {
                FindEntriesFromSet(actual, strPrefix, node.Entries, max, entries);
                return;
            }

            foreach (var n in node.Children)
            {
                if (entries.Count >= max)
                    return;
                prefix.Append(n.Key);
                FindEntriesFromNode(actual, n.Value, prefix, max, entries);
                prefix.Remove(prefix.Length - 1, 1);
            }
        }

        private void FindEntriesFromSet(string actual, string prefix,
            SortedSet<string> strings, int max, List<string> entries)
        {
            foreach (var s in strings)
            {
                if (entries.Count >= max)
                    return;
                string newEntry = prefix + s;
                if (newEntry.StartsWith(actual))
                    entries.Add(newEntry);
            }
        }

        public List<string> FuzzyPrefixSearch(string prefix, int max)
        {
            return PrefixSearch(prefix, max, true);
        }

        private void FuzzyPrefixSearch(string actual, TrieNode current, StringBuilder builder,
            int max, List<string> entries)
        {
            int len = Math.Min(actual.Length, builder.Length);
            string str = builder.ToString();
            if (actual.Length - builder.Length > MAX_LEN_DIFF ||
                LevenshteinDistance(str, actual.Substring(0, len)) > MAX_DIST)
                return;

            if (current.IsFullEntry)
                entries.Insert(0, str);

            if (current.IsBasic)
            {
                FuzzyFindEntriesFromSet(actual, 2, str, current.Entries, max, entries);
                return;
            }

            foreach (var node in current.Children)
            {
                if (entries.Count >= max)
                    return;
                builder.Append(node.Key);
                FuzzyPrefixSearch(actual, node.Value, builder, max, entries);
                builder.Remove(builder.Length - 1, 1);
            }
        }

        private void FuzzyFindEntriesFromSet(string actual, int maxDist, string prefix,
            SortedSet<string> strings, int max, List<string> entries)
        {
            foreach (var s in strings)
            {
                if (entries.Count >= max)
                    return;
                string newEntry = prefix + s;
                int newLen = Math.Min(actual.Length, newEntry.Length);
                if (actual.Length - newEntry.Length < MAX_LEN_DIFF &&
                    LevenshteinDistance(newEntry.Substring(0, newLen), actual) <= maxDist)
                    entries.Add(newEntry);
            }
        }

        private int LevenshteinDistance(string source, string target){
	        if(String.IsNullOrEmpty(source)){
		        if(String.IsNullOrEmpty(target)) return 0;
		        return target.Length;
	        }
	        if(String.IsNullOrEmpty(target)) return source.Length;
 
	        if(source.Length > target.Length){
		        var temp = target;
		        target = source;
		        source = temp;
	        }
 
	        var m = target.Length;
	        var n = source.Length;
	        var distance = new int[2, m + 1];
	        // Initialize the distance 'matrix'
	        for(var j = 1; j <= m; j++) distance[0, j] = j;
 
	        var currentRow = 0;
	        for(var i = 1; i <= n; ++i){
		        currentRow = i & 1;
		        distance[currentRow, 0] = i;
		        var previousRow = currentRow ^ 1;
		        for(var j = 1; j <= m; j++){
			        var cost = (target[j - 1] == source[i - 1] ? 0 : 1);
			        distance[currentRow, j] = Math.Min(Math.Min(
						        distance[previousRow, j] + 1,
						        distance[currentRow, j - 1] + 1),
						        distance[previousRow, j - 1] + cost);
		        }
	        }
	        return distance[currentRow, m];
        }

        private List<string> PrefixSearch(string prefix, int max, bool fuzzy)
        {
            var matches = new List<string>();
            var builder = new StringBuilder();
            var current = Traverse(prefix, builder);

            if (fuzzy)
                FuzzyPrefixSearch(prefix, current, builder, max, matches);
            else
                FindEntriesFromNode(prefix, current, builder, max, matches);

            return matches;
        }

        private TrieNode Traverse(string prefix, StringBuilder builder)
        {
            var current = root;

            foreach (char c in prefix)
            {
                if (current.IsLeaf || !current.Children.HasChild(c))
                    break;

                current = current.Children[c];
                builder.Append(c);
            }

            return current;
        }

        private class TrieNode
        {
            public AlphabetMap<TrieNode> Children { get; set; }
            public SortedSet<string> Entries { get; set; }
            public bool IsFullEntry { get; set; }
            public bool IsLeaf { get { return Children == null || Children.Count == 0; } }
            public bool IsBasic { get { return Entries != null; } }

            public TrieNode()
            {
                Entries = new SortedSet<string>();
            }

            public SortedSet<string> ConvertToFullTrieNode()
            {
                SortedSet<string> entries = Entries;
                Entries = null;
                Children = new AlphabetMap<TrieNode>();
                return entries;
            }
        }
    }
}