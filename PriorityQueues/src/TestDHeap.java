import static org.junit.Assert.*;

import java.util.Random;

import org.junit.Test;


public class TestDHeap {

	public static final double DELTA = 0.1;
	public static final int LARGE = 10000;

	/**
	 * Test method for {@link DHeap#DHeap(4)}.
	 */
	@Test
	public void testDHeap() {
		DHeap heap = new DHeap(100);
		assertTrue("Contructor failed to create empty heap", heap.size() == 0 && heap.isEmpty());
	}

	/**
	 * Test method for {@link DHeap#isEmpty()}.
	 */
	@Test
	public void testIsEmpty() {
		DHeap heap = new DHeap(100);
		heap.insert(50.0);
		
		// Test for not empty
		assertTrue("IsEmpty not working on filled heap", !heap.isEmpty());
		
		// Test for empty
		heap.deleteMin();
		assertTrue("IsEmpty not working on emptied heap", heap.isEmpty());
		
		// Test for long list
		addElements(heap, LARGE);
		assertTrue("IsEmpty not working on large filled heap", !heap.isEmpty());
		
		deleteElements(heap, LARGE);
		assertTrue("IsEmpty not working on large empty heap", heap.isEmpty());
	}

	/**
	 * Test method for {@link DHeap#size()}.
	 */
	@Test
	public void testSize() {
		DHeap heap = new DHeap(100);
		heap.insert(50.0);
		
		// Test for small list
		assertTrue("Size not working small filled heap", heap.size() == 1);
		
		heap.insert(50.0);
		assertTrue("Size not working small filled heap", heap.size() == 2);
		
		// Test for long list
		addElements(heap, LARGE);
		assertTrue("IsEmpty not working on large filled heap", heap.size() == LARGE + 2);
		
		deleteElements(heap, LARGE + 2);
		assertTrue("IsEmpty not working on large empty heap", heap.size() == 0);
	}

	/**
	 * Test method for {@link DHeap#findMin()}.
	 */
	@Test
	public void testFindMin() {
		DHeap heap = new DHeap(100);
		heap.insert(3);
		heap.insert(2);
		heap.insert(1);
		
		assertEquals("findMind does not work for small heap", heap.findMin(), 1.0, DELTA);
		heap.deleteMin();
		
		assertEquals("findMin does not work after removing min element", heap.findMin(), 2.0, DELTA);

		addMinElements(heap, LARGE, 1);
		heap.insert(0.5);
		
		assertEquals("findMin does not work for large heap", heap.findMin(), 0.5, DELTA);
		
		// Test exception
		heap.makeEmpty();
		try {
			heap.findMin();
			assertTrue("findMin does not throw exception for empty heap", false);
		} catch(Exception e) { }
	}

	/**
	 * Test method for {@link DHeap#insert(double)}.
	 */
	@Test
	public void testInsert() {
		DHeap heap = new DHeap(100);
		addElements(heap, LARGE);
		
		assertTrue(heap.size() == LARGE);
		heap.insert(3454);
		assertTrue(heap.size() == LARGE + 1);
	}

	/**
	 * Test method for {@link DHeap#deleteMin()}.
	 */
	@Test
	public void testDeleteMin() {
		DHeap heap = new DHeap(100);
		addElements(heap, LARGE);

		// test for removal of elements in order;
		double prev = heap.deleteMin();
		double current;
		while (!heap.isEmpty()) {
			current = heap.deleteMin();
			assertTrue("Larger element had priority in the heap", current >= prev);
			prev = current;
		}
		
		// Test exception
		heap.makeEmpty();
		try {
			heap.deleteMin();
			assertTrue("deleteMin does not throw exception for empty heap", false);
		} catch(Exception e) { }
	}

	/**
	 * Test method for {@link DHeap#makeEmpty()}.
	 */
	@Test
	public void testMakeEmpty() {
		DHeap heap = new DHeap(100);
		heap.makeEmpty();
		
		assertTrue("makeEmpty breaks empty heap", heap.isEmpty());
		
		heap.insert(50);
		heap.makeEmpty();
		assertTrue("makeEmpty does not work for small heap", heap.isEmpty());
		
		addElements(heap, LARGE);
		heap.makeEmpty();
		assertTrue("makeEmpty does not work for large heap", heap.isEmpty());
	}
	
	private void addElements(DHeap heap, int numElements) {
		addMinElements(heap, numElements, 0.0);
	}
	
	private static Random r = new Random(1);
	private void addMinElements(DHeap heap, int numElements, double min) {
		for (int i = numElements; i > 0; --i) {
			heap.insert(Math.max(min,r.nextInt(100)));
		}
	}
	
	private void deleteElements(DHeap heap, int numElements) {
		for (int i = numElements; i > 0; --i) {
			heap.deleteMin();
		}
	}

}
