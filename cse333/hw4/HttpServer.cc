/*
 * Copyright 2012 Steven Gribble
 *
 *  This file is part of the UW CSE 333 course project sequence
 *  (333proj).
 *
 *  333proj is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  333proj is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with 333proj.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>

#include "./FileReader.h"
#include "./HttpConnection.h"
#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpServer.h"
#include "./libhw3/QueryProcessor.h"

using std::cerr;
using std::cout;
using std::endl;
using hw3::QueryProcessor;

namespace hw4 {

// This is the function that threads are dispatched into
// in order to process new client connections.
void HttpServer_ThrFn(ThreadPool::Task *t);

// Given a request, produce a response.
HttpResponse ProcessRequest(const HttpRequest &req,
                            const std::string &basedir,
                            const std::list<std::string> *indices);

// Process a file request.
HttpResponse ProcessFileRequest(const std::string &uri,
                                const std::string &basedir);

// Process a query request.
HttpResponse ProcessQueryRequest(const std::string &uri,
                                 const std::list<std::string> *indices);

// Get the content-type string for the given file.
static std::string GetContentType(const std::string& fname);

// Helper method to get query terms from the given uri.
static std::vector<std::string> GetQueryTerms(const std::string& uri,
                                              std::string* query);

// Helper method to fill the given string with html query results.
static void FillHtmlResults(
    const std::string& query,
    const std::vector<QueryProcessor::QueryResult>& queryResults,
    std::string *content);

// Fills the top of the html page for search.
static void FillHtmlTop(std::string *content);

// Fills the bottom of the html page
static void FillHtmlBottom(std::string *content);

bool HttpServer::Run(void) {
  // Create the server listening socket.
  int listen_fd;
  cout << "  creating and binding the listening socket..." << endl;
  if (!ss_.BindAndListen(AF_UNSPEC, &listen_fd)) {
    cerr << endl << "Couldn't bind to the listening socket." << endl;
    return false;
  }

  // Spin, accepting connections and dispatching them.  Use a
  // threadpool to dispatch connections into their own thread.
  cout << "  accepting connections..." << endl << endl;
  ThreadPool tp(kNumThreads);
  while (1) {
    HttpServerTask *hst = new HttpServerTask(HttpServer_ThrFn);
    hst->basedir = staticfile_dirpath_;
    hst->indices = &indices_;
    if (!ss_.Accept(&hst->client_fd,
                    &hst->caddr,
                    &hst->cport,
                    &hst->cdns,
                    &hst->saddr,
                    &hst->sdns)) {
      // The accept failed for some reason, so quit out of the server.
      // (Will happen when kill command is used to shut down the server.)
      break;
    }
    // The accept succeeded; dispatch it.
    tp.Dispatch(hst);
  }
  return true;
}

void HttpServer_ThrFn(ThreadPool::Task *t) {
  // Cast back our HttpServerTask structure with all of our new
  // client's information in it.
  std::unique_ptr<HttpServerTask> hst(static_cast<HttpServerTask *>(t));
  cout << "  client " << hst->cdns << ":" << hst->cport << " "
       << "(IP address " << hst->caddr << ")" << " connected." << endl;

  HttpConnection conn(hst->client_fd);
  bool done = false;
  while (!done) {
    // Use the HttpConnection class to read in the next request from
    // this client, process it by invoking ProcessRequest(), and then
    // use the HttpConnection class to write the response.  If the
    // client sent a "Connection: close\r\n" header, then shut down
    // the connection.
    HttpRequest request;
    if (conn.GetNextRequest(&request)) {
      auto response = ProcessRequest(request, hst->basedir, hst->indices);
      conn.WriteResponse(response);
      if (request.headers["connection"] == "close") {
        done = true;
      }
    } else {
      done = true;
    }
  }

  cout << "  client " << hst->cdns << ":" << hst->cport << " "
       << "(IP address " << hst->caddr << ")" << " closed." << endl;
  close(hst->client_fd);
}

HttpResponse ProcessRequest(const HttpRequest &req,
                            const std::string &basedir,
                            const std::list<std::string> *indices) {
  // Is the user asking for a static file?
  if (req.URI.substr(0, 8) == "/static/") {
    return ProcessFileRequest(req.URI, basedir);
  }

  // The user must be asking for a query.
  return ProcessQueryRequest(req.URI, indices);
}


HttpResponse ProcessFileRequest(const std::string &uri,
                                const std::string &basedir) {
  // The response we'll build up.
  HttpResponse ret;

  // Steps to follow:
  //  - use the URLParser class to figure out what filename
  //    the user is asking for.
  //
  //  - use the FileReader class to read the file into memory
  //
  //  - copy the file content into the ret.body
  //
  //  - depending on the file name suffix, set the response
  //    Content-type header as appropriate, e.g.,:
  //      --> for ".html" or ".htm", set to "text/html"
  //      --> for ".jpeg" or ".jpg", set to "image/jpeg"
  //      --> for ".png", set to "image/png"
  //      etc.
  //
  // be sure to set the response code, protocol, and message
  // in the HttpResponse as well.
  std::string fname = "";

  URLParser parser;
  parser.Parse(uri);
  fname = parser.get_path();
  fname.replace(0, 8, "");

  std::string file_contents;
  FileReader freader(basedir, fname);
  if (freader.ReadFile(&file_contents)) {
    ret.headers["Content-type"] = GetContentType(fname);

    ret.protocol = "HTTP/1.1";
    ret.response_code = 200;
    ret.message = "Success";
    ret.body += file_contents;
    return ret;
  }

  // If you couldn't find the file, return an HTTP 404 error.
  ret.protocol = "HTTP/1.1";
  ret.response_code = 404;
  ret.message = "Not Found";
  ret.body = "<html><body>Couldn't find file \"";
  ret.body +=  EscapeHTML(fname);
  ret.body += "\"</body></html>";
  return ret;
}

HttpResponse ProcessQueryRequest(const std::string &uri,
                                 const std::list<std::string> *indices) {
  // The response we're building up.
  HttpResponse ret;

  // Your job here is to figure out how to present the user with
  // the same query interface as our solution_binaries/http333d server.
  // A couple of notes:
  //
  //  - no matter what, you need to present the 333gle logo and the
  //    search box/button
  //
  //  - if the user had previously typed in a search query, you also
  //    need to display the search results.
  //
  //  - you'll want to use the URLParser to parse the uri and extract
  //    search terms from a typed-in search query.  convert them
  //    to lower case.
  //
  //  - you'll want to create and use a hw3::QueryProcessor to process
  //    the query against the search indices
  //
  //  - in your generated search results, see if you can figure out
  //    how to hyperlink results to the file contents, like we did
  //    in our solution_binaries/http333d.
  ret.protocol = "HTTP/1.1";
  ret.response_code = 200;
  ret.message = "success";
  FillHtmlTop(&ret.body);

  std::string query;
  auto terms = GetQueryTerms(uri, &query);
  if (!terms.empty()) {
    hw3::QueryProcessor processor(*indices, false);
    auto results = processor.ProcessQuery(terms);
    if (!results.empty()) {
      FillHtmlResults(query, results, &ret.body);
    } else {
      ret.body += "No results found for <strong>";
      ret.body += query + "</strong>";
    }
  }

  FillHtmlBottom(&ret.body);


  return ret;
}

static std::string GetContentType(const std::string& fname) {
  if (boost::algorithm::ends_with(fname, ".html") ||
      boost::algorithm::ends_with(fname, ".htm")) {
    return "text/html";
  } else if (boost::algorithm::ends_with(fname, ".css")) {
    return "text/css";
  } else if (boost::algorithm::ends_with(fname, ".js")) {
    return "text/javascript";
  } else if (boost::algorithm::ends_with(fname, ".jpeg") ||
             boost::algorithm::ends_with(fname, ".jpg")) {
    return "image/jpeg";
  } else if (boost::algorithm::ends_with(fname, ".png")) {
    return "image/png";
  } else {
    return "text/plain";
  }
}

static std::vector<std::string> GetQueryTerms(const std::string& uri,
                                              std::string* query) {
  std::vector<std::string> results;
  URLParser parser;
  parser.Parse(uri);

  auto args = parser.get_args();
  if (args.count("terms") > 0) {
    boost::split(results, args["terms"], boost::is_any_of(" "),
                 boost::token_compress_on);
    *query = args["terms"];
  }

  return results;
}


static void FillHtmlResults(
    const std::string& query,
    const std::vector<QueryProcessor::QueryResult>& queryResults,
    std::string *content) {
  *content += "<p><br>";
  *content += std::to_string(queryResults.size());
  *content += " result(s) found for <strong>";
  *content += query + "</strong></p>";

  *content += "<ul>";
  for (const auto& result : queryResults) {
    *content += "<li><a href=\"";
    if (!boost::starts_with(result.document_name, "http://")) {
      *content += "/static/";
    }
    *content += result.document_name + "\">";
    *content += result.document_name;
    *content += "</a>";
    *content += " [" + std::to_string(result.rank) + "]";
  }
  *content += "</ul>";
}

static void FillHtmlTop(std::string *content) {
  *content = "<html><head><title>333gle</title></head><body>";
  *content += "<center style=\"font-size:500%;\">";
  *content += "<span style=\"position:relative;bottom:-0.33em;color:orange;\">";
  *content += "3</span>";
  *content += "<span style=\"color:red;\">3</span><span style=\"color:gold;\">";
  *content += "3</span>";
  *content += "<span style=\"color:blue;\">g";
  *content += "</span><span style=\"color:green;\">";
  *content += "l</span>";
  *content += "<span style=\"color:red;\">e</span>";
  *content += "</center><p><center>";
  *content += "<div style=\"height:20px;\"></div>";
  *content += "<form action=\"/query\" method=\"get\">";
  *content += "<input type=\"text\" size=30 name=\"terms\" />";
  *content += "<input type=\"submit\" value=\"Search\" />";
  *content += "</form></center><p>";
}

static void FillHtmlBottom(std::string *content) {
  *content += "</body></html>";
}

}  // namespace hw4
