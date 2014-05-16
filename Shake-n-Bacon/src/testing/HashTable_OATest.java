package testing;

import providedCode.DataCounter;
import shake_n_bacon.HashTable_OA;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu 
 * Represents the test for the open addressing hash table
 * implementation of the DataCounter.
 */
public class HashTable_OATest extends DataCounterTest {

	@Override
	public DataCounter createDataCounter() {
		return new HashTable_OA(DataCounterTest.COMPARATOR, DataCounterTest.HASHER);
	}

}
