package writeupExperiment;

import providedCode.DataCount;
import providedCode.DataCounter;
import shake_n_bacon.HashTable_OA;
import shake_n_bacon.HashTable_SC;
import shake_n_bacon.StringComparator;
import shake_n_bacon.StringHasher;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu 
 * The HashingExperiment program tests two different implementations of the
 * Hasher interface. Prints out the results + averages.
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
	 * Returns the average runtime for the given counter and file.
	 * @param counter the counter to test.
	 * @param file the file to load and test against.
	 * @return the average runtime.
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
