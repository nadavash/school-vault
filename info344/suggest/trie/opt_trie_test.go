package trie_test

import (
	"github.com/nadavash/school-vault/info344/suggest/trie"
	"io/ioutil"
	"os"
	"reflect"
	"strings"
	"testing"
)

func TestNewOptTrieConstructor(t *testing.T) {
	tr := trie.NewOptTrie()
	if tr == nil {
		t.Error("Trie constructor returned nil.")
	}

	if tr.Count() != 0 {
		t.Error("Trie should have count 0 after construction; has ", tr.Count(), " instead.")
	}
}

func TestOptTrieFromStream(t *testing.T) {
	tr, err := loadOptTrieFromFile()
	if err != nil {
		t.Fatal(err)
	}

	if tr == nil {
		t.Error("Trie constructor returned nil.")
	}

	if tr.Count() != resourceFileLineCount {
		t.Errorf("Trie should have count %d after construction; has %d instead.", resourceFileLineCount, tr.Count())
	}

	resCount := len(tr.PrefixSearch("blahblahblah", 0))
	if resCount != 0 {
		t.Errorf("Should return 0 results; returned instead %d", resCount)
	}

	resCount = len(tr.PrefixSearch("somebogusstring", 1))
	if resCount != 0 {
		t.Errorf("Should return 0 results; returned instead %d", resCount)
	}

	bribedExpected := []string{"bribed"}
	bribedActual := tr.PrefixSearch("bribed", 1)
	if !reflect.DeepEqual(bribedExpected, bribedActual) {
		t.Errorf("Expected: %v; actual: %v", bribedExpected, bribedActual)
	}

	yellExpected := []string{"yell", "yelled", "yeller", "yellers", "yelling"}
	yellActual := tr.PrefixSearch("yell", 5)
	if !reflect.DeepEqual(yellExpected, yellActual) {
		t.Errorf("Expected: %v; actual: %v", yellExpected, yellActual)
	}

	audExpected := []string{"aud", "audacious", "audaciously", "audaciousness", "audacities", "audacity", "audad", "audads", "audibility", "audible"}
	audActual := tr.PrefixSearch("aud", 10)
	if !reflect.DeepEqual(audExpected, audActual) {
		t.Errorf("Expected: %v; actual: %v", audExpected, audActual)
	}
}

func TestOptAddString(t *testing.T) {
	tr := trie.NewOptTrie()

	resCount := len(tr.PrefixSearch("hello", 1))
	if resCount != 0 {
		t.Errorf("Should return 0 results; returned instead %d", resCount)
	}

	tr.AddString("hello")
	if tr.Count() != 1 {
		t.Error("Trie Count() should return 1; returns ", tr.Count(), " instead.")
	}

	hellExpected := []string{"hello"}
	hellActual := tr.PrefixSearch("hel", 3)
	if !reflect.DeepEqual(hellExpected, hellActual) {
		t.Errorf("Expected: %v; actual: %v", hellExpected, hellActual)
	}

	tr.AddString("hello")
	if tr.Count() != 1 {
		t.Error("Trie Count() should return 1; returns ", tr.Count(), " instead.")
	}

	tr.AddString("hello world")

	helloExpected := []string{"hello", "hello world"}
	helloActual := tr.PrefixSearch("hell", 2)
	if !reflect.DeepEqual(helloExpected, helloActual) {
		t.Errorf("Expected: %v; actual: %v", helloExpected, helloActual)
	}
}

func BenchmarkOptPrefixSearch(b *testing.B) {
	file, err := os.Open(queryStringsFile)
	if err != nil {
		b.Fatal(err)
	}

	contents, err := ioutil.ReadAll(file)
	if err != nil {
		b.Fatal(err)
	}

	queries := strings.Split(string(contents), "\n")

	tr, err := loadOptTrieFromFile()
	if err != nil {
		b.Fatal(err)
	}

	for i := 0; i < b.N; i++ {
		tr.PrefixSearch(queries[i%len(queries)], 20)
	}
}

func loadOptTrieFromFile() (*trie.OptTrie, error) {
	file, err := os.Open(resourceFile)
	if err != nil {
		return nil, err
	}
	tr := trie.NewOptTrieFromStream(file)
	file.Close()

	return tr, nil
}
