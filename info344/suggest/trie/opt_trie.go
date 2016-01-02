package trie

import (
	"bufio"
	"io"
)

// OptTrie represents a tree data structure useful useful for performing
// a prefix search over a set of string values.
type OptTrie struct {
	root  *optNode
	count int32
}

// NewOptTrie creates an empty trie.
func NewOptTrie() *OptTrie {
	t := new(OptTrie)
	t.root = newOptNode(0)

	return t
}

// NewOptTrieFromStream creates a new trie with entries from the given |lines|
// stream with newline-separated entries.
func NewOptTrieFromStream(lines io.Reader) *OptTrie {
	t := NewOptTrie()
	scan := bufio.NewScanner(lines)
	for scan.Scan() {
		t.AddString(scan.Text())
	}

	return t
}

// Count returns the number of entries in the trie.
func (t *OptTrie) Count() int32 {
	return t.count
}

// AddString adds an entry to the try.
func (t *OptTrie) AddString(val string) {
	current := t.root

	index := 0
	// Iterate over each character in the string traverse the trie.
	for ; index < len(val); index++ {
		// If the character value exists in the current node, continue
		// traversing. Otherwise, create the necessary node.
		if next, ok := current.children.Get(val[index]); ok {
			current = next.(*optNode)
		} else {
			child := newOptNode(val[index])
			current.children.Put(val[index], child)
			current = child
		}
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
func (t *OptTrie) PrefixSearch(query string, max int) []string {
	current := t.root

	level := 0
	for ; level < len(query); level++ {
		if current.children.Len() == 0 {
			return nil
		}

		next, ok := current.children.Get(query[level])
		if !ok {
			return nil
		}
		current = next.(*optNode)
	}

	results := make([]string, 0, max)
	findEntriesInOptNode(current, query, max, &results)

	return results
}

func findEntriesInOptNode(node *optNode, prefix string, max int, results *[]string) {
	if node.isFullEntry {
		*results = append(*results, prefix)
	}

	node.children.ForEach(func(key byte, value interface{}) bool {
		if len(*results) >= max {
			return false
		}

		child := value.(*optNode)
		findEntriesInOptNode(child,
			prefix+string(child.value),
			max,
			results)

		return true
	})
}

type optNode struct {
	value       byte
	children    *AlphabetMap // byte -> *optNode
	isFullEntry bool
}

func newOptNode(value byte) *optNode {
	tn := new(optNode)
	tn.value = value
	tn.children = NewAlphabetMap()

	return tn
}
