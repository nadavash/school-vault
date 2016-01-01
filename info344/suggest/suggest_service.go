package main

import (
    "github.com/nadava/school-vault/info344/suggest/trie"
)

// SuggestService defines service methods for search query completion.
type SuggestService struct {
    prefixTree trie.Trie
}

// Hello returns a 'hello' message.
func (s *SuggestService) Hello(arg *struct{}, reply *string) error {
    *reply = "Hello, world!"
    return nil
}
