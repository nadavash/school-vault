package testing;

import providedCode.DataCounter;
import shake_n_bacon.HashTable_SC;

/**
 * TODO
 * @author nadavash
 *
 */
public class HashTable_SCTest extends DataCounterTest {

	@Override
	public DataCounter createDataCounter() {
		return new HashTable_SC(DataCounterTest.COMPARATOR, DataCounterTest.HASHER);
	}

}
