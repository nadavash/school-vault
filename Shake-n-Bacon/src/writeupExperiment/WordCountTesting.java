package writeupExperiment;

import java.io.IOException;

import providedCode.*;
import shake_n_bacon.*;

/**
 * An executable that counts the words in a files and prints out the counts in
 * descending order. You will need to modify this file.
 */
public class WordCountTesting {

	/**
	 * Creates an array of DataCount objects from a specified DataCounter object.
	 * @param counter the object to get the data from.
	 * @return The array of elements from the counter object.
	 */
	public static DataCount[] getCountsArray(DataCounter counter) {
		SimpleIterator ite = counter.getIterator();
		DataCount[] counts = new DataCount[counter.getSize()];
		
		int index = 0;
		while (ite.hasNext()) {
			counts[index++] = ite.next();
		}
		
		return counts;
	}

	// ////////////////////////////////////////////////////////////////////////
	// /////////////// DO NOT MODIFY ALL THE METHODS BELOW ///////////////////
	// ////////////////////////////////////////////////////////////////////////

	public static void countWords(String file, DataCounter counter) {
		try {
			FileWordReader reader = new FileWordReader(file);
			String word = reader.nextWord();
			int count = 0;
			while (word != null) {
				counter.incCount(word);
				word = reader.nextWord();
				count++;
			}			
		} catch (IOException e) {
			System.err.println("Error processing " + file + " " + e);
			System.exit(1);
		}
	}


	/*
	 * Print error message and exit
	 */
	private static void usage() {
		System.err
				.println("Usage: [-s | -o] <filename of document to analyze>");
		System.err.println("-s for hashtable with separate chaining");
		System.err.println("-o for hashtable with open addressing");
		System.exit(1);
	}

	/**
	 * Entry of the program
	 * 
	 * @param args
	 *            the input arguments of this program
	 */
	public static void main(String[] args) {
		if (args.length != 2) {
			usage();
		}

		String firstArg = args[0].toLowerCase();
		DataCounter counter = null;
		if (firstArg.equals("-s")) {
			counter = new HashTable_SC(new StringComparator(),
					new StringHasher());
		} else if (firstArg.equals("-o")) {
			counter = new HashTable_OA(new StringComparator(),
					new StringHasher());
		} else {
			usage();
		}

		countWords(args[1], counter);
		DataCount[] counts = getCountsArray(counter);
	}
}