/*
 * Copyright 2011 Steven Gribble
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

#ifndef _HW3_QUERYPROCESSOR_H_
#define _HW3_QUERYPROCESSOR_H_

#include <cstdio>
#include <list>
#include <vector>
#include <string>
#include <memory>

#include "./fileindexutil.h"
#include "./DocIDTableReader.h"
#include "./DocTableReader.h"
#include "./FileIndexReader.h"
#include "./IndexTableReader.h"

using std::list;
using std::vector;
using std::string;

namespace hw3 {

// A QueryProcessor is a class that is given a set of names of index
// files, and uses the various FileIndexReader and HashTableReader
// classes to process queries against the indices.
class QueryProcessor {
 public:
  // Construct a QueryProcessor.  Arguments:
  //
  // - indexlist: a std::list<std::string> containing a list of index
  //   filenames that the QueryProcessor should use.
  //
  // - validate: a bool indicating whether or not to validate the
  //   checksums in the index files.  Defaults to true.
  QueryProcessor(list<string> indexlist, bool validate = true);

  // The destructor.
  ~QueryProcessor();

  // This structure defines a single query result.  As with HW2,
  // the rank of a query result is the sum of the number of occurrences
  // of query words within the document.
  class QueryResult {
   public:
    bool operator<(const QueryResult &rhs) const { return rank > rhs.rank; }
    string    document_name;  // The name of a matching document.
    HWSize_t  rank;           // The rank of the matching document.
  };

  // This method processes a query against the indices and returns a
  // vector of QueryResults, sorted in descending order of rank.  If no
  // documents match the query, then a valid but empty vector will be
  // returned.
  vector<QueryResult> ProcessQuery(const vector<string> &query);

 protected:
  // The list of index files we process.
  list<string> indexlist_;

  // The arrays of pointers to DocTableReader and IndexTableReader
  // objects.
  HWSize_t            arraylen_;
  DocTableReader    **dtr_array_;
  IndexTableReader  **itr_array_;

 private:
  class DocIDResult {
   public:
    DocID_t   docid;  // The name of a matching document.
    HWSize_t  rank;   // The rank of the matching document.
  };

  // A private helper to get DocIDTableReaders from the indices for the given
  // string.
  void ProcessQuery(const vector<string>& query, HWSize_t index,
                    list<QueryProcessor::QueryResult>* results);

  // A private helper to populate results from the given DocIDTableReaders.
  void PopulateResults(DocIDTableReader& reader, HWSize_t index,
                       list<DocIDResult>* results);

  // A private helper to cross reference between two query results list and
  // ensure only results from both stay.
  void CrossReferenceResults(DocIDTableReader& reader,
                             list<DocIDResult>* final);

  void ConvertToQueryResults(const list<DocIDResult>& idResults, HWSize_t index,
                             list<QueryProcessor::QueryResult>* queryResults);

  // Disable the copy constructor and assignment operator.
  DISALLOW_COPY_AND_ASSIGN(QueryProcessor);
};

}  // namespace hw3

#endif  // _HW3_QUERYPROCESSOR_H_
