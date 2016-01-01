package trie

import (
	"bytes"
)

// AlphabetMap represents a map with a possible key range from a-z and a space
// character.
type AlphabetMap struct {
	elements []alphabetMapNode
	count    int
}

// NewAlphabetMap creates an empty map with available range of keys from a-z
// and a space character.
func NewAlphabetMap() *AlphabetMap {
	return &AlphabetMap{
		elements: make([]alphabetMapNode, 27),
		count:    0,
	}
}

// ForEach iterates over the keys in the AlphabetMap with none-nil values and
// calls the given function |f| with the byte key and the corresponding value.
// Stops iterating if |f| returns false.
func (am *AlphabetMap) ForEach(f func(key byte, value interface{}) bool) {
	for index, node := range am.elements {
		if node.isSet && !f(indexToLetter(index), node.value) {
			return
		}
	}
}

// Get returns the corresponding value for the given |char| key. Returns nil
// if no value exists for the corresponding key.
func (am *AlphabetMap) Get(char byte) (interface{}, bool) {
	index := letterToIndex(char)
	if index != -1 {
		return am.elements[index].value, am.elements[index].isSet
	}

	return nil, false
}

// Put adds the given |value| for the given |char| key.
func (am *AlphabetMap) Put(char byte, value interface{}) {
	index := letterToIndex(char)
	if index != -1 {
		am.elements[index].isSet = true
		am.elements[index].value = value
	}
}

func letterToIndex(char byte) int {
	if char == ' ' {
		return 26
	} else if 'a' <= char && char <= 'z' {
		return int(bytes.ToLower([]byte{char})[0] - 'a')
	}

	return -1
}

func indexToLetter(index int) byte {
	if index == 26 {
		return ' '
	} else if 0 <= index && index <= 26 {
		return 'a' + byte(index)
	}

	return 0
}

type alphabetMapNode struct {
	isSet bool
	value interface{}
}
