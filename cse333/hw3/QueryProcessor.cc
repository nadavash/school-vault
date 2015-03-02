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

  // Go through each index
  for (HWSize_t i = 0; i < arraylen_; ++i) {
    list<QueryProcessor::QueryResult> indexResults;
    ProcessQuery(query, i, &indexResults);
    finalresult.insert(finalresult.end(), indexResults.begin(),
                       indexResults.end());
  }

  // Sort the final results.
  std::sort(finalresult.begin(), finalresult.end());
  return finalresult;
}

void QueryProcessor::ProcessQuery(const vector<string>& query, HWSize_t index,
                                  list<QueryProcessor::QueryResult>* results) {
  Verify333(index < arraylen_);
  Verify333(results != nullptr);
  Verify333(!query.empty());

  list<DocIDResult> tempResults;

  std::unique_ptr<DocIDTableReader> reader(
      itr_array_[index]->LookupWord(query[0]));
  if (reader != nullptr) {
    PopulateResults(*reader, index, &tempResults);
  } else {
    return;
  }

  for (auto iter = ++query.begin(); iter != query.end(); ++iter) {
    std::cout << *iter << std::endl;
    std::unique_ptr<DocIDTableReader> currReader(
        itr_array_[index]->LookupWord(*iter));
    if (reader == nullptr) {
      return;
    }

    CrossReferenceResults(*currReader, &tempResults);
    if (tempResults.empty()) {
      return;
    }
  }

  ConvertToQueryResults(tempResults, index, results);
}

void QueryProcessor::PopulateResults(DocIDTableReader& reader, HWSize_t index,
    list<DocIDResult>* results) {
  for (const auto& docid : reader.GetDocIDList()) {
    list<DocPositionOffset_t> occurences;
    if (reader.LookupDocID(docid.docid, &occurences)) {
      DocIDResult result;
      result.docid = docid.docid;
      result.rank = occurences.size();
      results->push_back(result);
    }
  }
}

void QueryProcessor::CrossReferenceResults(DocIDTableReader& reader,
    list<DocIDResult>* final) {
  for (auto iter = final->begin(); iter != final->end();) {
    list<DocPositionOffset_t> occurences;
    if (reader.LookupDocID(iter->docid, &occurences)) {
      iter->rank += occurences.size();
      ++iter;
    } else {
      iter = final->erase(iter);
    }
  }
}

void QueryProcessor::ConvertToQueryResults(
    const list<DocIDResult>& idResults, HWSize_t index,
    list<QueryProcessor::QueryResult>* queryResults) {
  for (const auto& idResult : idResults) {
    QueryProcessor::QueryResult queryResult;
    if (!dtr_array_[index]->LookupDocID(idResult.docid,
                                        &queryResult.document_name)) {
      continue;
    }
    queryResult.rank = idResult.rank;
    queryResults->push_back(queryResult);
  }
}

}  // namespace hw3
