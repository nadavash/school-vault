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

#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"

using std::map;
using std::string;

namespace hw4 {

bool HttpConnection::GetNextRequest(HttpRequest *request) {
  // Use "WrappedRead" to read data into the buffer_
  // instance variable.  Keep reading data until either the
  // connection drops or you see a "\r\n\r\n" that demarcates
  // the end of the request header.
  //
  // Once you've seen the request header, use ParseRequest()
  // to parse the header into the *request argument.
  //
  // Very tricky part:  clients can send back-to-back requests
  // on the same socket.  So, you need to preserve everything
  // after the "\r\n\r\n" in buffer_ for the next time the
  // caller invokes GetNextRequest()!
  const int buf_size = 64;
  unsigned char buf[buf_size];

  const std::string header = "\r\n\r\n";

  int res;
  size_t end_index;
  while ((end_index = buffer_.find(header)) == std::string::npos) {
    res = WrappedRead(fd_, buf, sizeof(buf));
    if (res < 0) {
      return false;
    } else if (res == 0) {
      break;
    }

    buffer_.append(reinterpret_cast<char *>(buf), (size_t)res);
  }

  end_index = end_index + header.size() - 1;
  *request = ParseRequest(end_index);

  return true;
}

bool HttpConnection::WriteResponse(const HttpResponse &response) {
  std::string str = response.GenerateResponseString();
  int res = WrappedWrite(fd_,
                         (unsigned char *) str.c_str(),
                         str.length());
  if (res != static_cast<int>(str.length()))
    return false;
  return true;
}

HttpRequest HttpConnection::ParseRequest(size_t end) {
  HttpRequest req;
  req.URI = "/";  // by default, get "/".

  // Get the header.
  std::string str = buffer_.substr(0, end);

  // Split the header into lines.  Extract the URI from the first line
  // and store it in req.URI.  For each additional line beyond the
  // first, extract out the header name and value and store them in
  // req.headers (i.e., req.headers[headername] = headervalue).
  // You should look at HttpResponse.h for details about the HTTP header
  // format that you need to parse.
  //
  // You'll probably want to look up boost functions for (a) splitting
  // a string into lines on a "\r\n" delimiter, (b) trimming
  // whitespace from the end of a string, and (c) converting a string
  // to lowercase.
  std::vector<std::string> lines;

  boost::split(lines, str, boost::is_any_of("\r\n"), boost::token_compress_on);
  if (lines.size() > 0) {
    std::vector<std::string> parts;
    boost::split(parts, str, boost::is_any_of(" "), boost::token_compress_on);
    if (parts.size() >= 2) {
      req.URI = parts[1];
    }
  }

  for (auto iter = ++lines.begin(); iter != lines.end(); ++iter) {
    boost::algorithm::to_lower(*iter);
    std::vector<std::string> parts;
    boost::split(parts, *iter, boost::is_any_of(":"), boost::token_compress_on);
    if (parts.size() >= 2) {
      req.headers[boost::trim_copy(parts[0])] = boost::trim_copy(parts[1]);
    }
  }

  if (end >= buffer_.size()) {
    buffer_.clear();
  } else {
    buffer_ = buffer_.substr(end);
  }

  return req;
}

}  // namespace hw4
