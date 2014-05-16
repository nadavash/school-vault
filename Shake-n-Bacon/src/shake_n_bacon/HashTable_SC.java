package shake_n_bacon;

import providedCode.*;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu
 * This HashTable class represents a class the implements a hash table
 * using the separate chaining method for handling collisions. This is meant
 * to be used for counting the frequencies of a different strings. The table
 * can grow up to a size of around 250,000 data/count pairs.
 */
public class HashTable_SC extends DataCounter {
	
	private TableNode[] table;
	private int sizeIndex;
	private int size;
	private Comparator<String> comparator;
	private Hasher hasher;

	public HashTable_SC(Comparator<String> c, Hasher h) {
		sizeIndex = 0;
		table = new TableNode[Helpers.TABLE_SIZES[sizeIndex]];
		hasher = h;
		comparator = c;
	}

	@Override
	public void incCount(String data) {		
		int index = getIndex(data);
		TableNode node = searchBucket(index, data);

		// If the data is not found in the bucket, create a new one. 
		if (node == null) {
			// If the table is relatively full, and there is still room to expand,
			// grow the table
			if (size >= table.length && sizeIndex < Helpers.TABLE_SIZES.length - 1) {
				growTable();
				index = getIndex(data);
			}
			
			size++;
			node = new TableNode(data, table[index]);
			table[index] = node;
		}
		
		node.count++;
	}

	@Override
	public int getSize() {
		return size;
	}

	@Override
	public int getCount(String data) {
		TableNode node = searchBucket(getIndex(data), data);
		return node == null ? 0 : node.count;
	}

	@Override
	public SimpleIterator getIterator() {
		return new SCIterator();
	}
	
	/**
	 * Calculates the index for the given string.
	 * @param data the string to hash and index.
	 * @return the index generated.
	 */
	private int getIndex(String data) {
		return hasher.hash(data) % table.length;
	}
	
	/**
	 * Searches for the specified data at the specified bucket in the table.
	 * Otherwise returns null.
	 * @param index the index of the bucket to search.
	 * @param data the data element to search for
	 * @return the node if found. Otherwise returns null.
	 */
	private TableNode searchBucket(int index, String data) {
		TableNode node = table[index];
		while (node != null) {
			if (comparator.compare(data, node.key) == 0)
				return node;
			
			node = node.next;
		}
		
		return null;
	}
	
	/**
	 * Inserts a given node into the specified table.
	 * @param node the node to insert.
	 * @param table the table to insert into.
	 */
	private void insertNode(TableNode node, TableNode[] table) {
		int index = hasher.hash(node.key) % table.length;
		node.next = table[index];
		table[index] = node;
	}
	
	/**
	 * Grows the table to the next available size and moves over all
	 * the data with it.
	 */
	private void growTable() {
		int newSize = Helpers.TABLE_SIZES[++sizeIndex];
		TableNode[] newTable = new TableNode[newSize];
		SimpleIterator ite = getIterator();
		
		while (ite.hasNext()) {
			DataCount current = ite.next();
			TableNode node = new TableNode(current.data, current.count);
			insertNode(node, newTable);
		}

		table = newTable;
	}
	
	/**
	 * Represents an iterator that iterates over a separate-chaining hash table
	 * implementation.
	 */
	private class SCIterator implements SimpleIterator {
		private TableNode current;
		private int bucketIndex;
		private int nextCount;
		
		/**
		 * Creates a new instance of the separate chaining table iterator.
		 */
		public SCIterator() {
			gotoNextBucket();
		}

		@Override
		public DataCount next() {
			++nextCount;
			DataCount res = new DataCount(current.key, current.count);
			current = current.next;
			
			if (current == null && hasNext()) {
				++bucketIndex;
				gotoNextBucket();
			}
			
			return res;
		}

		@Override
		public boolean hasNext() {
			return nextCount < size;
		}
		
		/**
		 * Moves the iterator to the next bucket until a new DataCount is found.
		 */
		private void gotoNextBucket() {
			while (bucketIndex < table.length) {
				if (table[bucketIndex] != null) {
					current = table[bucketIndex];
					return;
				}
				
				bucketIndex++;
			}
		}
	}
	
	/**
	 * The TableNode class represents a node used in the separate chaining
	 * hash table.
	 */
	private static class TableNode {
		public String key;
		public int count;
		public TableNode next;
		
		/**
		 * Creates a new table node.
		 * @param key the key to set in the node.
		 * @param count the count.
		 */
		public TableNode(String key, int count) {
			this.key = key;
			this.count = count;
		}
		
		/**
		 * Creates a new table node with a key and the next node.
		 */
		public TableNode(String key, TableNode next) {
			this(key, 0);
			this.next = next;
		}
	}
}