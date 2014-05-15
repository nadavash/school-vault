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
 *        1. You may implement HashTable with open addressing discussed in
 *        class; You can choose one of those three: linear probing, quadratic
 *        probing or double hashing. The only restriction is that it should not
 *        restrict the size of the input domain (i.e., it must accept any key)
 *        or the number of inputs (i.e., it must grow as necessary).
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
public class HashTable_OA extends DataCounter {
	public static final int[] SIZES = new int[] {
		37
	};
	
	private DataCount[] table;
	private int sizeIndex;
	private Comparator<String> comparator;
	private Hasher hasher;
	
	public HashTable_OA(Comparator<String> c, Hasher h) {
		sizeIndex = 0;
		table = new DataCount[SIZES[sizeIndex]];
		comparator = c;
		hasher = h;
	}

	@Override
	public void incCount(String data) {
		// TODO Auto-generated method stub
	}

	@Override
	public int getSize() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int getCount(String data) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public SimpleIterator getIterator() {
		// TODO Auto-generated method stub
		return null;
	}
	
	// Returns an index based on the specified string key if the
	// key exists in the table. Otherwise returns -1.
	private int getIndex(String data) {
		int startIndex = hasher.hash(data) % table.length;
		int probeIndex = 0;
		
		int i = 0;
		while (table[probeIndex] != null &&
				table[probeIndex].count >= 0) {
			
			if (table[probeIndex].data.equals(data))
				return probeIndex;
	
			++i;
			probeIndex = startIndex + i * i;
		}
		
		return -1;
	}

}