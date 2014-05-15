/**
 * 
 */
package testing;

import static org.junit.Assert.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

import org.junit.Before;
import org.junit.Test;

import providedCode.*;
import shake_n_bacon.*;

/**
 * @author nadavash
 * TODO
 */
public abstract class DataCounterTest {
	
	public static final Hasher HASHER = new StringHasher();
	public static final Comparator<String> COMPARATOR = new StringComparator();
	public static final String TEST_TEXT_FILE = "words.txt";
	public static final ArrayList<String> WORDS = new ArrayList<String>();
		
	/**
	 * Creates a new DataCounter for the current test instance.
	 * @return a new DataCounter instance.
	 */
	public abstract DataCounter createDataCounter();
	
	@Before
	public void setUp() {		
		if (WORDS.size() > 0)
			return;
		
		// Populate the arrayList once
		try {
			Scanner s = new Scanner(new File(TEST_TEXT_FILE));
			int ss = 0;
			while (s.hasNext()) {
				WORDS.add(s.next());
				ss++;
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
		counter = createDataCounter();
		for (int i = 0; i < WORDS.size(); ++i) {
			counter.incCount(WORDS.get(i));
		}
		
		assertEquals("GetSize does not work for large quantities.", counter.getSize(), WORDS.size());
	}


	@Test
	public void testGetCount() {
		DataCounter counter = createDataCounter();
		for (int i = 0; i < 50; ++i) {
			counter.incCount("hello");
		}
		
		assertEquals("GetCount does not return correct amount.", counter.getCount("hello"), 50);
		
		// Increment count for many words
		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 10; ++j) {
				counter.incCount(WORDS.get(i));
			}
		}
		
		// Check counts for many words
		for (int i = 0; i < 100; ++i) {
			assertEquals("GetCount does not work for many words.", counter.getCount(WORDS.get(i)), 10);
		}
	}


	@Test
	public void testGetIterator() {
		fail("Not yet implemented");
	}

}
