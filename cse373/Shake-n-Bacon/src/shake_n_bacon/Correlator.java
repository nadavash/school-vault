package shake_n_bacon;

import java.io.IOException;

import providedCode.*;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu
 * The correlator program takes two files and analyzes their word frequencies
 * to produce a variance value. It accepts three arguments: -S|-O flags to specify
 * which type of hash table to use, and the paths to the two files following that.
 */
public class Correlator {
	public static final double FREQUENCY_MIN = 0.0001;
	public static final double FREQUENCY_MAX = 0.01;

	public static void main(String[] args) {
		double variance = 0.0;
		
		if (args.length != 3) {
			usage();
		}

		String firstArg = args[0].toLowerCase();
		DataCounter counter1 = null;
		DataCounter counter2 = null;
		if (firstArg.equals("-s")) {
			counter1 = new HashTable_SC(new StringComparator(),
					new StringHasher());
			counter2 = new HashTable_SC(new StringComparator(),
					new StringHasher());
		} else if (firstArg.equals("-o")) {
			counter1 = new HashTable_OA(new StringComparator(),
					new StringHasher());
			counter2 = new HashTable_SC(new StringComparator(),
					new StringHasher());
		} else {
			usage();
		}
		
		int total1 = countWords(args[1], counter1);
		int total2 = countWords(args[2], counter2);
		variance = calculateVariance(counter1, counter2, total1, total2);
		
		// IMPORTANT: Do not change printing format. Just print the double.
		System.out.println(variance);
	}
	
	/*
	 * Print error message and exit
	 */
	private static void usage() {
		System.err
				.println("Usage: [-s | -o] <filename 2> <filename 2>");
		System.err.println("-s for hashtable with separate chaining");
		System.err.println("-o for hashtable with open addressing");
		System.exit(1);
	}
	
	/**
	 * Populates the given data counter with the words from the specified
	 * file.
	 * @param file the file to get words from.
	 * @param counter the counter to populate.
	 * @return the total number of words contained in the file.
	 */
	public static int countWords(String file, DataCounter counter) {
		int words = 0;
		try {
			FileWordReader reader = new FileWordReader(file);
			String word = reader.nextWord();
			while (word != null) {
				counter.incCount(word);
				word = reader.nextWord();
				words++;
			}
		} catch (IOException e) {
			System.err.println("Error processing " + file + " " + e);
			System.exit(1);
		}
		
		return words;
	}
	
	/**
	 * Compares the word frequencies in the specified counter and calculate a word
	 * variance between the two. This ignores words not found in both, and ignores
	 * frequencies sufficiently small or large.
	 * @param counter1 the first counter to examine.
	 * @param counter2 the second counter to examine.
	 * @param total1 the total word count in the file correlating to the first counter.
	 * @param total2 the total word count in the file correlating to the second counter.
	 * @return a variance between the two counters.
	 */
	public static double calculateVariance(DataCounter counter1, DataCounter counter2,
			int total1, int total2) {
		double sum = 0;
		
		SimpleIterator ite = counter1.getIterator();
		while (ite.hasNext()) {
			DataCount next = ite.next();
			double frequency = (double)next.count / total1;
			
			int count2 = counter2.getCount(next.data);
			// If frequency is too low or high, or the word is not found
			// in both counters, continue to the next word.
			if (!valid(frequency) || count2 == 0)
				continue;
			
			double frequency2 = (double)count2 / total2;
			
			// Same thing for the second frequency
			if (!valid(frequency2))
				continue;
			
			// Get the square value to add to the sum
			frequency = frequency - frequency2;
			sum += frequency * frequency;
		}
		
		return sum;
	}
	
	/**
	 * Checks if the given word frequency is valid for variance calculation.
	 * @param frequency the frequency to check.
	 * @return true if the frequency is within a valid range, false otherwise.
	 */
	public static boolean valid(double frequency) {
		return frequency <= FREQUENCY_MAX && frequency >= FREQUENCY_MIN;
	}
}
