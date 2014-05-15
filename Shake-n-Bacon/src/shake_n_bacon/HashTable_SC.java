package shake_n_bacon;

import providedCode.*;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu
 * 
 *        TODO: Replace this comment with your own as appropriate.
 * 
 *        1. You may implement HashTable with separate chaining discussed in
 *        class; the only restriction is that it should not restrict the size of
 *        the input domain (i.e., it must accept any key) or the number of
 *        inputs (i.e., it must grow as necessary).
 * 
 *        2. Your HashTable should rehash as appropriate (use load factor as
 *        shown in the class).
 * 
 *        3. To use your HashTable for WordCount, you will need to be able to
 *        hash strings. Implement your own hashing strategy using charAt and
 *        length. Do NOT use Java's hashCode method.
 * 
 *        4. HashTable should be able to grow at least up to 200,000. We are not
 *        going to test input size over 200,000 so you can stop resizing there
 *        (of course, you can make it grow even larger but it is not necessary).
 * 
 *        5. We suggest you to hard code the prime numbers. You can use this
 *        list: http://primes.utm.edu/lists/small/100000.txt NOTE: Make sure you
 *        only hard code the prime numbers that are going to be used. Do NOT
 *        copy the whole list!
 * 
 *        TODO: Develop appropriate tests for your HashTable.
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
			if (size >= table.length && sizeIndex < Helpers.TABLE_SIZES.length - 1)
				growTable();
			
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
	
	// TODO
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
	
	public void print() {
		int count = 0;
		for (int i = 0; i < table.length; ++i) {
			TableNode current = table[i];
			while (current != null) {
				System.out.println((count++) + ". " + current.key + ": " + current.count);
				current = current.next;
			}	
		}
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
		 * TODO
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
		
		// TODO
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
		 * TODO
		 * @param key
		 * @param count
		 */
		public TableNode(String key, int count) {
			this.key = key;
			this.count = count;
		}
		
		/**
		 * TODO
		 * @param count
		 * @param next
		 */
		public TableNode(String key, TableNode next) {
			this(key, 0);
			this.next = next;
		}
	}
}