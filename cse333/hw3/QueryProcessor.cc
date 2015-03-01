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

#include <iostream>
#include <algorithm>

#include "./QueryProcessor.h"

extern "C" {
  #include "./libhw1/CSE333.h"
}

namespace hw3 {

QueryProcessor::QueryProcessor(list<string> indexlist, bool validate) {
  // Stash away a copy of the index list.
  indexlist_ = indexlist;
  arraylen_ = indexlist_.size();
  Verify333(arraylen_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader *[arraylen_];
  itr_array_ = new IndexTableReader *[arraylen_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::iterator idx_iterator = indexlist_.begin();
  for (HWSize_t i = 0; i < arraylen_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = new DocTableReader(fir.GetDocTableReader());
    itr_array_[i] = new IndexTableReader(fir.GetIndexTableReader());
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (HWSize_t i = 0; i < arraylen_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string> &query) {
  Verify333(query.size() > 0);
  vector<QueryProcessor::QueryResult> finalresult;

  list<DocIDResult> tempResults;
  std::unique_ptr<DocIDTableReader> reader(ProcessQueryPart(query[0]));
  if (reader == nullptr) {
    return finalresult;
  }
  PopulateResults(*reader, &tempResults);

  for (auto iter = ++query.begin(); iter != query.end(); ++iter) {
    std::unique_ptr<DocIDTableReader> currReader(ProcessQueryPart(*iter));
    if (currReader == nullptr) {
      return finalresult;
    }

    CrossReferenceResults(*currReader, &tempResults);
    if (tempResults.empty()) {
      return finalresult;
    }
  }

  for (const auto& queryRes : tempResults) {
    QueryResult result;
    result.rank = queryRes.rank;
    if (GetDocName(queryRes.docid, &result.document_name)) {
      finalresult.push_back(result);
    }
  }

  // Sort the final results.
  std::sort(finalresult.begin(), finalresult.end());
  return finalresult;
}

DocIDTableReader* QueryProcessor::ProcessQueryPart(const string& str) {
  for (int i = 0; i < arraylen_; ++i) {
    std::unique_ptr<DocIDTableReader> reader(itr_array_[i]->LookupWord(str));
    if (reader != nullptr) {
      return reader.release();
    }
  }
  return nullptr;
}

// A private helper to com
void QueryProcessor::PopulateResults(DocIDTableReader& reader,
                                     list<DocIDResult>* results) {
  for (auto docid : reader.GetDocIDList()) {
    list<DocPositionOffset_t> occurences;
    if (!reader.LookupDocID(docid.docid, &occurences))
      continue;

    DocIDResult result;
    result.docid = docid.docid;
    result.rank = occurences.size();
    results->push_back(result);
  }
}

bool QueryProcessor::GetDocName(const DocID_t& docid, string* name) {
  for (int i = 0; i < arraylen_; ++i) {
    if (dtr_array_[i]->LookupDocID(docid, name)) {
      return true;
    }
  }

  return false;
}

void QueryProcessor::CrossReferenceResults(DocIDTableReader& reader,
                                           list<DocIDResult>* final) {
  for (auto iter = final->begin(); iter != final->end(); iter++) {
    list<DocPositionOffset_t> occurences;
    if (reader.LookupDocID(iter->docid, &occurences)) {
      iter->rank += occurences.size();
    } else {
      iter = final->erase(iter);
    }
  }
}

}  // namespace hw3
