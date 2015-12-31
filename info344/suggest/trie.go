package main

import (
    "bufio"
    "io"
)

type Trie struct {
    root *trieNode
    count int32
}

func NewTrie() *Trie {
    t := new(Trie)
    t.root = newTrieNode(0)

    return t
}

func NewTrieFromStream(lines io.Reader) *Trie {
    t := NewTrie()
    scan := bufio.NewScanner(lines)
    for scan.Scan() {
        t.AddString(scan.Text())
    }

    return t
}

func (t *Trie) Count() int32 {
    return t.count
}

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

func (t *Trie) PrefixSearch(query string, max int) []string {
    current := t.root

    level := 0
    for char := range query {
        if len(current.children) == 0 {
            return nil
        }

        next, ok := current.children[query[char]]
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

    for char, node := range node.children {
        if len(*results) >= max {
            return
        }

        findEntriesInNode(node, prefix + string(char), max, results)
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