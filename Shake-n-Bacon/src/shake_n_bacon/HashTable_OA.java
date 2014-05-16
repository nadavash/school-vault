package shake_n_bacon;

import providedCode.*;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu
 * The HashTable_OA class represents a has table that stores strings
 * as keys with counts as values. The open addressing system uses a
 * quadratic probing mechanism and thus remains at least half empty
 * at all times. The table can handle input of up to around 250,000
 * data/count pairs.
 */
public class HashTable_OA extends DataCounter {
	public static final int SIZE_FACTOR = 2;

	private DataCount[] table;
	private int sizeIndex;
	private int size;
	private Comparator<String> comparator;
	private Hasher hasher;
	
	public HashTable_OA(Comparator<String> c, Hasher h) {
		sizeIndex = 0;
		table = new DataCount[Helpers.TABLE_SIZES[sizeIndex] * SIZE_FACTOR];
		comparator = c;
		hasher = h;
	}

	@Override
	public void incCount(String data) {
		int index = getIndex(data);

		// If not found in the array add it!
		if (table[index] == null) {
			size++;

			// Grow the array if necessary
			if (size >= table.length / SIZE_FACTOR) {
				resize();
				index = getIndex(data);
			}
			
			table[index] = new DataCount(data, 0);
		}

		table[index].count++;
	}

	@Override
	public int getSize() {
		return size;
	}

	@Override
	public int getCount(String data) {
		int index = getIndex(data);
		return table[index] == null ? 0 : table[index].count;
	}

	@Override
	public SimpleIterator getIterator() {
		return new OAIterator();
	}
	
	/**
	 * Calculates the index in the hash table for the given data string.
	 * @param data the data string to examine.
	 * @return the index for the data string.
	 */
	private int getIndex(String data) {
		int startIndex = hasher.hash(data) % table.length;
		int probeIndex = startIndex;
		
		int i = 0;
		while (table[probeIndex] != null && 
				comparator.compare(table[probeIndex].data, data) != 0) {
			++i;
			probeIndex = (startIndex + i * i) % table.length;
		}
		
		return probeIndex;
	}
	
	/**
	 * Resizes the table array of this OA hash table and copies over all
	 * of the data from the previous table.
	 */
	private void resize() {
		DataCount[] prev = table;
		table = new DataCount[Helpers.TABLE_SIZES[sizeIndex++] * SIZE_FACTOR];
		for (int i = 0; i < prev.length; ++i) {
			if (prev[i] != null) {			
				int index = getIndex(prev[i].data);
				table[index] = prev[i];
			}
		}
	}
	
	/**
	 * Represents a SimpleIterator that iterates over an open addressing
	 * hash table.
	 */
	private class OAIterator implements SimpleIterator {
		private int currentIndex;
		private int nextCount;
		
		/**
		 * Creates a new open addressing iterator.
		 */
		public OAIterator() {
			findNext();
		}
		
		@Override
		public DataCount next() {
			nextCount++;
			DataCount res = table[currentIndex++];
			// Make a copy of the datacount to avoid data corruption by client
			res = new DataCount(res.data, res.count);
			if (hasNext())
				findNext();
			
			return res;
		}

		@Override
		public boolean hasNext() {
			return nextCount < size;
		}
		
		/**
		 * Finds the next element in the table if the current one is null.
		 */
		private void findNext() {
			while (currentIndex < table.length && table[currentIndex] == null) {
				++currentIndex;
			}
		}
	}

}