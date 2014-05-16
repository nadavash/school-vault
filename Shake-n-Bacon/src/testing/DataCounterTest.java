/**
 * 
 */
package testing;

import static org.junit.Assert.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;

import org.junit.Before;
import org.junit.Test;

import providedCode.*;
import shake_n_bacon.*;

/**
 * @author nadavash
 * TODO write test description
 */
public abstract class DataCounterTest {
	
	public static final Hasher HASHER = new StringHasher();
	public static final Comparator<String> COMPARATOR = new StringComparator();
	public static final String TEST_TEXT_FILE = "words.txt";
	public static final Set<String> WORDS = new TreeSet<String>();
		
	private DataCounter large;
	
	/**
	 * Creates a new DataCounter for the current test instance.
	 * @return a new DataCounter instance.
	 */
	public abstract DataCounter createDataCounter();
	
	@Before
	public void setUp() {		
		if (WORDS.size() > 0) {
			large = createDataCounter();
			for (String s : WORDS) {
				large.incCount(s);
			}
			return;
		}
		
		// Populate the arrayList once
		try {
			Scanner s = new Scanner(new File(TEST_TEXT_FILE));
			while (s.hasNext()) {
				WORDS.add(s.next());
			}
			s.close();
			
			large = createDataCounter();
			for (String st : WORDS) {
				large.incCount(st);
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Test
	public void testIncCount() {
		DataCounter counter = createDataCounter();
		counter.incCount("hello");
		
		assertTrue("First increment does not work.", counter.getCount("hello") == 1);
		counter.incCount("hello");
		
		assertTrue("Second increment does not work.", counter.getCount("hello") == 2);
		
		counter.incCount("goodbye");
		assertTrue("Increment does not work for second word.", counter.getCount("goodbye") == 1);
		
		for (int i = 0; i < 49; ++i) {
			counter.incCount("goodbye");
		}
		
		assertTrue("Increment does not work for many increments.", counter.getCount("goodbye") == 50);
	}


	@Test
	public void testGetSize() {
		DataCounter counter = createDataCounter();
		assertEquals("GetSize does not work for empty.", counter.getSize(), 0);
		counter.incCount("hello");
		assertEquals("GetSize does not work for 1st insertion.", counter.getSize(), 1);
		counter.incCount("hello");
		assertEquals("GetSize returns one higher for incrementing existing value.", counter.getSize(), 1);
		
		// Test for big input
		assertEquals("GetSize does not work for large quantities.", large.getSize(), WORDS.size());
	}


	@Test
	public void testGetCount() {
		DataCounter counter = createDataCounter();
		for (int i = 0; i < 50; ++i) {
			counter.incCount("hello");
		}
		
		assertEquals("GetCount does not return correct amount.", counter.getCount("hello"), 50);
		counter = createDataCounter();
		
		// Increment count for many words
		for (String word : WORDS) {
			//System.out.println("Before " + counter.getCount(word));

			for (int j = 0; j < 10; ++j) {
				counter.incCount(word);
				assertEquals("Not counting correctly.", counter.getCount(word), j + 1);
			}
			
			//System.out.println("After " + counter.getCount(word));
		}
				
		// Check counts for many words
		for (String word : WORDS) {
			assertEquals("GetCount does not work for many words.", counter.getCount(word), 10);
		}
	}


	@Test
	public void testGetIterator() {
		DataCounter counter = createDataCounter();
		counter.incCount("hello");
		counter.incCount("goodbye");
		counter.incCount("mazeltov");
		
		SimpleIterator ite = counter.getIterator();
		
		int count = 0;
		while (ite.hasNext()) {
			ite.next();
			count++;
		}
		
		assertEquals("Iterator does not return three items.", count, 3);
		
		ite = large.getIterator();
		count = 0;
		while (ite.hasNext()) {
			DataCount next = ite.next();
			boolean found = WORDS.contains(next.data);
			assertEquals("Count is not correct for DataCount iterator.", next.count, 1);
			assertTrue("Not all words returned are contained within the word list.", found);
			++count;
		}
		
		assertEquals("Iterator does not enough items for large list.", count, WORDS.size());
	}

}
