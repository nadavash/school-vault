package main

import (
	"errors"
	"github.com/nadavash/school-vault/info344/suggest/trie"
	"io"
	"strings"
)

// SuggestArgs specifies contains arguments for the Suggest service call.
// For max number of queries, 1 <= max <= 25.
type SuggestArgs struct {
	query string
	max int
}

// SuggestService defines service methods for search query completion.
type SuggestService struct {
	suggestTrie *trie.OptTrie
}

// NewSuggestService is a constructor for creating a SuggestService object
// with suggest entries from the given stream.
func NewSuggestService(stream io.Reader) *SuggestService {
	return &SuggestService{
		suggestTrie: trie.NewOptTrieFromStream(stream),
	}
}

// Suggest returns the desired number of query suggestions for a given string query.
func (s *SuggestService) Suggest(args *SuggestArgs, reply *[]string) error {
	if 1 > args.max || args.max > 25 {
		return errors.New("max must be greater than 1 and less than 25")
	}

	if len(args.query) == 0 {
		*reply = []string{}  // Empty suggestion set.
		return nil
	}

	args.query = strings.ToLower(args.query)
	*reply = s.suggestTrie.PrefixSearch(args.query, args.max)

	return nil
}
