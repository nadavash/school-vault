package trie

import (
    "bufio"
    "io"
    "sort"
)

// Trie represents a tree data structure useful useful for performing
// a prefix search over a set of string values.
type Trie struct {
    root *trieNode
    count int32
}

// NewTrie creates an empty trie.
func NewTrie() *Trie {
    t := new(Trie)
    t.root = newTrieNode(0)

    return t
}

// NewTrieFromStream creates a new trie with entries from the given |lines|
// stream with newline-separated entries.
func NewTrieFromStream(lines io.Reader) *Trie {
    t := NewTrie()
    scan := bufio.NewScanner(lines)
    for scan.Scan() {
        t.AddString(scan.Text())
    }

    return t
}

// Count returns the number of entries in the trie.
func (t *Trie) Count() int32 {
    return t.count
}

// AddString adds an entry to the try.
func (t *Trie) AddString(val string) {
    current := t.root

    index := 0
    // Iterate over each character in the string traverse the trie.
    for index < len(val) {
        // If the character value exists in the current node, continue
        // traversing. Otherwise, create the necessary node.
        if next, ok := current.children[val[index]]; ok {
            current = next
        } else {
            child := newTrieNode(val[index])
            current.children[val[index]] = child
            current = child
        }

        index++
    }

    // Increase our counter only if the added string value didn't exist
    // before.
    if !current.isFullEntry {
        current.isFullEntry = true
        t.count++
    }
}

// PrefixSearch performs a prefix searcs over the trie with the given |query|
// and returns up to |max| number of matching results.
func (t *Trie) PrefixSearch(query string, max int) []string {
    current := t.root

    level := 0
    for level < len(query) {
        if len(current.children) == 0 {
            return nil
        }

        next, ok := current.children[query[level]]
        if !ok {
            return nil
        }
        current = next
        level++
    }

    results := make([]string, 0, max)
    findEntriesInNode(current, query, max, &results)

    return results
}

func findEntriesInNode(node *trieNode, prefix string, max int, results *[]string) {
    if node.isFullEntry {
        *results = append(*results, prefix)
    }

    keys := make([]int, 0, len(node.children))
    for key := range node.children {
        keys = append(keys, int(key))
    }
    sort.Ints(keys)

    for _, key := range keys {
        if len(*results) >= max {
            return
        }

        findEntriesInNode(node.children[byte(key)],
                          prefix + string(node.children[byte(key)].value),
                          max,
                          results)
    }
}

type trieNode struct {
    value byte
    children map[byte]*trieNode
    isFullEntry bool
}

func newTrieNode(value byte) *trieNode {
    tn := new(trieNode)
    tn.value = value
    tn.children = make(map[byte]*trieNode)

    return tn
}