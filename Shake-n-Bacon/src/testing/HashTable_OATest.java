package testing;

import providedCode.DataCounter;
import shake_n_bacon.HashTable_OA;

/**
 * 
 * @author nadavash
 * TODO
 */
public class HashTable_OATest extends DataCounterTest {

	@Override
	public DataCounter createDataCounter() {
		return new HashTable_OA(DataCounterTest.COMPARATOR, DataCounterTest.HASHER);
	}

}
