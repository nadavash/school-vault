package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net"
	"net/http"
	"net/rpc"
	"os"
	"strconv"
)

var entriesFile = flag.String("entries_file", "~/Downloads/words.txt", "Filepath to entries text file.")

var suggest *SuggestService

func suggestHandler(w http.ResponseWriter, r *http.Request) {
	log.Println("In 'suggestHandler.")
	var suggestions []string

	num, err := strconv.ParseInt(r.URL.Query().Get("n"), 10, 32)
	if err != nil {
		w.Write([]byte(err.Error()))
		return
	}

	err = suggest.Suggest(&SuggestArgs{r.URL.Query().Get("q"), int(num)}, &suggestions)
	if err != nil {
		w.Write([]byte(err.Error()))
		return
	}

	w.Header().Add("content-type", "application/json")
	err = json.NewEncoder(w).Encode(suggestions)
	if err != nil {
		log.Println(err)
	}
}

func main() {
	flag.Parse()

	file, err := os.Open(*entriesFile)
	if err != nil {
		log.Fatalln(err)
	}
	suggest = NewSuggestService(file)

	fmt.Println("Running Suggest!")
	rpc.Register(suggest)
	rpc.HandleHTTP()
	l, e := net.Listen("tcp", ":8008")
	if e != nil {
		log.Fatalln("listen error:", e)
	}

	http.HandleFunc("/suggest", suggestHandler)
	http.Handle("/", http.FileServer(http.Dir("web")))

	go http.Serve(l, nil)
	log.Fatalln(http.ListenAndServe(":80", nil))
}
