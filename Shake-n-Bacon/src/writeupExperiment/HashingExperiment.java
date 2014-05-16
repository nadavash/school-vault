package writeupExperiment;

import providedCode.DataCount;
import providedCode.DataCounter;
import shake_n_bacon.HashTable_OA;
import shake_n_bacon.HashTable_SC;
import shake_n_bacon.StringComparator;
import shake_n_bacon.StringHasher;

/**
 * TODO comment
 * @author nadavash
 *
 */
public class HashingExperiment {
	public static final String[] TEST_FILES = new String[] { "macbeth.txt",
		"the-new-atlantis.txt", "hamlet.txt", "words.txt" };
	public static final int NUM_TESTS = 10;
	public static final int NUM_WARMUP = 2;
	
	public static void main(String[] args) {
		for (String file : TEST_FILES) {
			double res = getAverageRuntime(new HashTable_SC(new StringComparator(),
					new StringHasher()), file);
			System.out.println(res);
			res = getAverageRuntime(new HashTable_SC(new StringComparator(),
					new BadStringHasher()), file);
			System.out.println(res);
		}
	}
	
	/**
	 * TODO
	 * 
	 * @param args
	 * @return
	 */
	private static double getAverageRuntime(DataCounter counter, String file) {
		double totalTime = 0;
		for (int i = 0; i < NUM_TESTS; i++) {
			long startTime = System.currentTimeMillis();
			runTest(counter, file);
			long endTime = System.currentTimeMillis();
			if (NUM_WARMUP <= i) { // Throw away first NUM_WARMUP runs to
									// encounter JVM warmup
				totalTime += (endTime - startTime);
				System.out.print((endTime - startTime) + "\t");
			}
		}

		return totalTime / (NUM_TESTS - NUM_WARMUP); // Return average runtime.
	}
	
	public static void runTest(DataCounter counter, String file) {
		WordCountTesting.countWords(file, counter);
		DataCount[] counts = WordCountTesting.getCountsArray(counter);
	}
}
