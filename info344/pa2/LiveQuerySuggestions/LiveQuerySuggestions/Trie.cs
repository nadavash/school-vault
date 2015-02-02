using System;
using System.Collections.Generic;
using System.IO;
using System.Web;

namespace LiveQuerySuggestions
{
    /// <summary>
    /// 
    /// </summary>
    public class Trie
    {
        private TrieNode root;
        private HashSet<string> set;

        public Trie()
        {
            root = new TrieNode('\0');
            set = new HashSet<string>();
        }

        public void ParseFromFile(StreamReader reader)
        {
            string word;
            while ((word = reader.ReadLine()) != null)
            {
                Add(word);
            }
        }

        public void Add(string word)
        {
            if (word == string.Empty)
                return;

            TrieNode current = root;
            foreach (char c in word)
            {
                if (current.Children[c] == null)
                {
                    current.Children[c] = new TrieNode(c);
                }
                current = current.Children[c];
            }
            current.FullEntry = word;
        }

        public List<string> PrefixSearch(string prefix)
        {
            var matches = new List<string>();   
            var path = new Stack<TrieNode>();
            var current = root;
            foreach (char c in prefix)
            {
                if (current.IsLeaf || current.Children[c] == null)
                    break;

                path.Push(current);
                current = current.Children[c];
            }


            FindEntriesFrom(current, 5, matches);

            return matches;
        }

        private void FindEntriesFrom(TrieNode node, int max, List<string> entries)
        {
            Queue<TrieNode> nodes = new Queue<TrieNode>();
            nodes.Enqueue(node);

            while (nodes.Count != 0 && entries.Count < max)
            {
                TrieNode current = nodes.Dequeue();
                if (current.IsFullEntry)
                    entries.Add(current.FullEntry);

                if (!current.IsLeaf)
                {
                    foreach (TrieNode n in current.Children)
                        nodes.Enqueue(n);
                }
            }
        }

        private class TrieNode
        {
            private AlphabetMap<TrieNode> children;

            public char Key { get; set; }
            public AlphabetMap<TrieNode> Children { 
                get
                {
                    if (children == null)
                        children = new AlphabetMap<TrieNode>();
                    return children;
                }
            }
            public string FullEntry { get; set; }
            public bool IsFullEntry { get { return FullEntry != null; } }
            public bool IsLeaf { get { return Children.Count == 0; } }

            public TrieNode(char key)
                : this(key, null) { }

            public TrieNode(char key, string entry)
            {
                Key = key;
                FullEntry = entry;
            }
        }
    }
}