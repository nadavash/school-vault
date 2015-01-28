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

extern "C" {
  #include "./CSE333.h"
  #include "./HashTable.h"
  #include "./HashTable_priv.h"
  #include "./LinkedList.h"
  #include "./LinkedList_priv.h"
}
#include "./test_suite.h"
#include "./test_hashtable.h"

namespace hw1 {

// TODO(nadavash): Test hash functions.
TEST_F(Test_HashTable, HTTestHashFunctions) {
  // Test FNVHash64
  unsigned char test_str[] = "hello!!!";
  ASSERT_EQ(FNVHash64(test_str, 8), FNVHash64(test_str, 8));

  uint64_t intVal = *((uint64_t *)test_str);
  // Test FNVHashInt64
  ASSERT_EQ(FNVHashInt64((HTValue_t)intVal), FNVHashInt64((HTValue_t)intVal));

  ASSERT_EQ(FNVHash64(test_str, 8), FNVHashInt64((HTValue_t)intVal));
  ASSERT_NE(FNVHash64(test_str, 4), FNVHashInt64((HTValue_t)intVal));
}

}  // namespace hw1
