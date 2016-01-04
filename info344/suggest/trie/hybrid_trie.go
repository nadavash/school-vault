package trie

import (
	"bufio"
	"github.com/emirpasic/gods/sets/treeset"
	"io"
	"strings"
)

const hybridNodeMaxChildren int = 100

type HybridTrie struct {
	root   hybridNode
	length int
}

func NewHybridTrie() *HybridTrie {
	return &HybridTrie{
		&leafNode{treeset.NewWithStringComparator(), false},
		0,
	}
}

func NewHybridTrieFromStream(lines io.Reader) *HybridTrie {
	t := NewHybridTrie()
	scan := bufio.NewScanner(lines)
	for scan.Scan() {
		t.AddString(scan.Text())
	}

	return t
}

func (t *HybridTrie) Count() int {
	return t.length
}

func (t *HybridTrie) AddString(entry string) {
	if len(entry) == 0 {
		return
	}

	var newEntry bool
	t.root, newEntry = t.root.Add(entry)
	if newEntry {
		t.length++
	}
}

func (t *HybridTrie) PrefixSearch(query string, max int) []string {
	res := make([]string, 0, 5)
	t.root.Find(query, max, 0, &res)
	return res
}

type hybridNode interface {
	Add(entry string) (hybridNode, bool)
	Find(prefix string, max int, pos int, results *[]string)
}

type leafNode struct {
	children    *treeset.Set // TreeSet<string>
	isFullEntry bool
}

func (l *leafNode) Add(entry string) (hybridNode, bool) {
	if len(entry) == 0 {
		newEntry := !l.isFullEntry
		l.isFullEntry = true
		return l, newEntry
	} else if l.children.Size() >= hybridNodeMaxChildren {
		var ln hybridNode = &linkNode{NewAlphabetMap(), l.isFullEntry}
		for _, childEntry := range l.children.Values() {
			ln, _ = ln.Add(childEntry.(string))
		}

		return ln.Add(entry)
	}

	newEntry := !l.children.Contains(entry)
	if newEntry {
		l.children.Add(entry)
	}
	return l, newEntry
}

func (l *leafNode) Find(prefix string, max int, pos int, results *[]string) {
	if len(prefix) == 0 || len(*results) >= max {
		return
	} else if l.isFullEntry && pos == len(prefix) {
		*results = append(*results, prefix)
	}

	for _, entry := range l.children.Values() {
		if len(*results) >= max {
			return
		}

		strEntry := entry.(string)
		if strings.HasPrefix(strEntry, prefix[pos:]) {
			*results = append(*results, prefix[:pos]+strEntry)
		}
	}
}

type linkNode struct {
	children    *AlphabetMap // map[byte]hybridNode
	isFullEntry bool
}

func (l *linkNode) Add(entry string) (hybridNode, bool) {
	if len(entry) == 0 {
		newEntry := !l.isFullEntry
		l.isFullEntry = true
		return l, newEntry
	}

	value, ok := l.children.Get(entry[0])
	var child hybridNode
	if ok {
		child = value.(hybridNode)
	} else {
		child = &leafNode{treeset.NewWithStringComparator(), false}
	}

	child, newEntry := child.Add(entry[1:])
	l.children.Put(entry[0], child)

	return l, newEntry
}

func (l *linkNode) Find(prefix string, max int, pos int, results *[]string) {
	if pos < len(prefix) {
		val, ok := l.children.Get(prefix[pos])
		if ok {
			val.(hybridNode).Find(prefix, max, pos+1, results)
		}
	} else {
		if l.isFullEntry {
			*results = append(*results, prefix)
		}

		l.children.ForEach(func(key byte, value interface{}) bool {
			if len(*results) >= max {
				return false
			}

			value.(hybridNode).Find(prefix+string(key), max, pos+1, results)
			return true
		})
	}
}
