package main

import (
    "fmt"
    "log"
    "net"
    "net/http"
    "net/rpc"
)

var suggest = new(SuggestService)

func handler(w http.ResponseWriter, r *http.Request) {
    var resp string
    suggest.Hello(nil, &resp)
    w.Write([]byte(resp))
}

func main() {
    fmt.Println("Hello, world!")
    rpc.Register(suggest)
    rpc.HandleHTTP()
    l, e := net.Listen("tcp", ":8008")
    if e != nil {
        log.Fatal("listen error:", e)
    }

    http.HandleFunc("/Hello", handler)

    go http.Serve(l, nil)
    http.ListenAndServe(":8080", nil)
}
