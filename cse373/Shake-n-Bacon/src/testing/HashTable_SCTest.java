package testing;

import providedCode.DataCounter;
import shake_n_bacon.HashTable_SC;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu 
 * Represents the JUnit test for the separate chaining implementation
 * of the hash tables for data counter.
 */
public class HashTable_SCTest extends DataCounterTest {

	@Override
	public DataCounter createDataCounter() {
		return new HashTable_SC(DataCounterTest.COMPARATOR, DataCounterTest.HASHER);
	}

}
