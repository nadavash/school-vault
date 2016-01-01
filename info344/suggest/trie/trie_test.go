package trie

import (
    "testing"
)

func TestTrieConstructor(t *testing.T) {
    tr := NewTrie()
    if tr == nil {
        t.Error("Trie constructor returned nil.")
    }

    if tr.root == nil {
        t.Error("Trie root is nil.")
    }
}

func TestAddString(t *testing.T) {
    tr := NewTrie()
    tr.AddString("hello")

    if tr.Count() != 1 {
        t.Error("Trie Count() should return 1; returns ", tr.Count(), " instead.")
    }

    tr.AddString("hello")
    if tr.Count() != 1 {
        t.Error("Trie Count() should return 1; returns ", tr.Count(), " instead.")
    }
}
