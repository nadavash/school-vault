package trie_test

import (
	"github.com/nadava/school-vault/info344/suggest/trie"
	"testing"
)

func TestTrieConstructor(t *testing.T) {
	tr := trie.NewTrie()
	if tr == nil {
		t.Error("Trie constructor returned nil.")
	}

	if tr.Count() != 0 {
		t.Error("Trie should have count 0 after construction; has ", tr.Count(), " instead.")
	}
}

func TestAddString(t *testing.T) {
	tr := trie.NewTrie()
	tr.AddString("hello")

	if tr.Count() != 1 {
		t.Error("Trie Count() should return 1; returns ", tr.Count(), " instead.")
	}

	tr.AddString("hello")
	if tr.Count() != 1 {
		t.Error("Trie Count() should return 1; returns ", tr.Count(), " instead.")
	}
}
