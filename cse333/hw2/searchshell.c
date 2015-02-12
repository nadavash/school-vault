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

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "libhw1/CSE333.h"
#include "memindex.h"
#include "filecrawler.h"

static void Usage(void);

// Converts the given string str to lowercase.
static void StrToLower(char *str, uint32_t len);

// Splits the given string str by a space delimiter and returns an array of
// strings with the delimited parts.
static uint8_t SplitSpaces(char *str, char **query);

// Prints all of th ematches from the given linked list, with the document names
// given by doctable.
static void PrintMatches(LinkedList matches, DocTable doctable);

int main(int argc, char **argv) {
  if (argc != 2)
    Usage();

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - crawl from a directory provided by argv[1] to produce and index
  //  - prompt the user for a query and read the query from stdin, in a loop
  //  - split a query into words (check out strtok_r)
  //  - process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  int retval;
  MemIndex index = NULL;
  DocTable doctable = NULL;

  printf("Indexing '%s'\n", argv[1]);
  retval = CrawlFileTree(argv[1], &doctable, &index);
  if (!retval) {
    fprintf(stderr, "Error while crawling filetree at '%s'", argv[1]);
    return EXIT_FAILURE;
  }

  char line[256];
  char *query[128];
  uint8_t len;
  LinkedList matches;

  while (1) {
    printf("enter query:\n");
    if (!fgets(line, sizeof(line), stdin))
      break;

    StrToLower(line, sizeof(line));
    len = SplitSpaces(line, query);
    matches = MIProcessQuery(index, query, len);
    if (matches) {
      PrintMatches(matches, doctable);
      FreeLinkedList(matches, free);
    }
  }

  FreeMemIndex(index);
  FreeDocTable(doctable);

  return EXIT_SUCCESS;
}

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(-1);
}

static void StrToLower(char *str, uint32_t len) {
  for (int i = len; i >= 0; --i) {
    str[i] = tolower(str[i]);
  }
}

static uint8_t SplitSpaces(char *str, char **query) {
  uint8_t i = 0;

  char *ch = strtok(str, " \n");
  while (ch != NULL) {
    if (i >= 128)
      break;

    query[i] = ch;
    ++i;

    ch = strtok(NULL, " \n");
  }

  return i;
}

static void PrintMatches(LinkedList results, DocTable doctable) {
  LLIter iter = LLMakeIterator(results, 0);
  if (!iter)
    return;

  bool next = true;
  while (next) {
    LLPayload_t payload;
    SearchResultPtr sr;
    char *docname;

    LLIteratorGetPayload(iter, &payload);
    sr = (SearchResultPtr) payload;

    docname = DTLookupDocID(doctable, sr->docid);

    if (docname) {
      printf("  %s (%" PRIu32 ")\n", docname, sr->rank);
    }

    next = LLIteratorNext(iter);
  }
}
