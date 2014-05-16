package writeupExperiment;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu 
 * TODO write header comment
 */
public class TimingExperiment {
	public static final String[] TEST_FILES = new String[] { "hamlet.txt",
			"the-new-atlantis.txt", "macbeth.txt", "words.txt" };
	public static final int NUM_TESTS = 10;
	public static final int NUM_WARMUP = 2;

	public static void main(String[] args) {
		String[] testArgs = new String[] { "-O", TEST_FILES[0] };
		String[] testArgs2 = new String[] { "-S", TEST_FILES[0] };
		for (String file : TEST_FILES) {
			testArgs[1] = testArgs2[1] = file;
			double res = getAverageRuntime(testArgs);
			System.out.println(res);
			res = getAverageRuntime(testArgs);
			System.out.println(res);
		}
	}

	/**
	 * TODO
	 * 
	 * @param args
	 * @return
	 */
	private static double getAverageRuntime(String[] args) {
		double totalTime = 0;
		for (int i = 0; i < NUM_TESTS; i++) {
			long startTime = System.currentTimeMillis();
			WordCountTesting.main(args);
			long endTime = System.currentTimeMillis();
			if (NUM_WARMUP <= i) { // Throw away first NUM_WARMUP runs to
									// encounter JVM warmup
				totalTime += (endTime - startTime);
				System.out.print((endTime - startTime) + "\t");
			}
		}

		return totalTime / (NUM_TESTS - NUM_WARMUP); // Return average runtime.
	}

}
