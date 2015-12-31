package main

import (
    "bufio"
    "fmt"
    // "log"
    "os"
    // "net"
    "net/http"
    // "net/rpc"
    "strings"
)

var suggest = new(SuggestService)

func handler(w http.ResponseWriter, r *http.Request) {
    var resp string
    suggest.Hello(nil, &resp)
    w.Write([]byte(resp))
}

func main() {
    fmt.Println("Hello, world!")
    reader := bufio.NewReader(os.Stdin)

    words, err := os.Open("C:\\Users\\Nadav\\Downloads\\words.txt")
    if err != nil {
        panic(err.Error())
    }
    trie := NewTrieFromStream(words)
    fmt.Println(trie.Count())

    for {
        input, _ := reader.ReadString('\n')
        if input == "\\q" {
            return
        }
        fmt.Println(trie.PrefixSearch(strings.TrimSpace(input), 5))
    }

    /*rpc.Register(suggest)
    rpc.HandleHTTP()
    l, e := net.Listen("tcp", ":8008")
    if e != nil {
        log.Fatal("listen error:", e)
    }

    http.HandleFunc("/Hello", handler)

    go http.Serve(l, nil)
    http.ListenAndServe(":8080", nil)*/
}
