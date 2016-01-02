package main

import (
	"bufio"
	"fmt"
	"github.com/nadava/school-vault/info344/suggest/trie"
	"os"
	"strings"
)

func main() {
	fmt.Println("Hello, world!")
	reader := bufio.NewReader(os.Stdin)

	words, err := os.Open("C:\\Users\\Nadav\\Downloads\\words.txt")
	if err != nil {
		panic(err.Error())
	}
	trie := trie.NewOptTrieFromStream(words)
	fmt.Println(trie.Count())

	for {
		fmt.Print(" > ")
		input, _ := reader.ReadString('\n')
		if input == "\\q" {
			return
		}
		fmt.Println(trie.Count())
		fmt.Println(trie.PrefixSearch(strings.TrimSpace(input), 10))
	}
}
