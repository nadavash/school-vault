package Testing;

import static org.junit.Assert.*;

import java.util.Random;

import org.junit.Test;

import PriorityQueues.GenericBinaryHeap;

public class TestGenericBinaryHeap {

	public static final double DELTA = 0.1;
	public static final int LARGE = 10000;
	
	/**
	 * Test method for {@link GenericBinaryHeap<Double, Double>#GenericBinaryHeap<Double, Double>()}.
	 */
	@Test
	public void testGenericBinaryHeap() {
		GenericBinaryHeap<Double, Double> heap = new GenericBinaryHeap<Double, Double>();
		assertTrue("Contructor failed to create empty heap", heap.size() == 0 && heap.isEmpty());
	}

	/**
	 * Test method for {@link GenericBinaryHeap<Double, Double>#isEmpty()}.
	 */
	@Test
	public void testIsEmpty() {
		GenericBinaryHeap<Double, Double> heap = new GenericBinaryHeap<Double, Double>();
		heap.insert(50.0, 0.5);
		
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
	 * Test method for {@link GenericBinaryHeap<Double, Double>#size()}.
	 */
	@Test
	public void testSize() {
		GenericBinaryHeap<Double, Double> heap = new GenericBinaryHeap<Double, Double>();
		heap.insert(50.0, 0.5);
		
		// Test for small list
		assertTrue("Size not working small filled heap", heap.size() == 1);
		
		heap.insert(50.0, 0.5);
		assertTrue("Size not working small filled heap", heap.size() == 2);
		
		// Test for long list
		addElements(heap, LARGE);
		assertTrue("IsEmpty not working on large filled heap", heap.size() == LARGE + 2);
		
		deleteElements(heap, LARGE + 2);
		assertTrue("IsEmpty not working on large empty heap", heap.size() == 0);
	}

	/**
	 * Test method for {@link GenericBinaryHeap<Double, Double>#findMin()}.
	 */
	@Test
	public void testFindMin() {
		GenericBinaryHeap<Double, Double> heap = new GenericBinaryHeap<Double, Double>();
		heap.insert(3.0, 3.0);
		heap.insert(2.0, 2.0);
		heap.insert(1.0, 1.0);
		
		assertEquals("findMind does not work for small heap", heap.findMin(), 1.0, DELTA);
		heap.deleteMin();
		
		assertEquals("findMin does not work after removing min element", heap.findMin(), 2.0, DELTA);

		addMinElements(heap, LARGE, 1);
		heap.insert(0.5, 0.5);
		
		assertEquals("findMin does not work for large heap", heap.findMin(), 0.5, DELTA);
		
		// Test exception
		heap.makeEmpty();
		try {
			heap.findMin();
			assertTrue("findMin does not throw exception for empty heap", false);
		} catch(Exception e) { }
	}

	/**
	 * Test method for {@link GenericBinaryHeap<Double, Double>#insert(double)}.
	 */
	@Test
	public void testInsert() {
		GenericBinaryHeap<Double, Double> heap = new GenericBinaryHeap<Double, Double>();
		addElements(heap, LARGE);
		
		assertTrue(heap.size() == LARGE);
		heap.insert(3454.0, 3454.0);
		assertTrue(heap.size() == LARGE + 1);
	}

	/**
	 * Test method for {@link GenericBinaryHeap<Double, Double>#deleteMin()}.
	 */
	@Test
	public void testDeleteMin() {
		GenericBinaryHeap<Double, Double> heap = new GenericBinaryHeap<Double, Double>();
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
	 * Test method for {@link GenericBinaryHeap<Double, Double>#makeEmpty()}.
	 */
	@Test
	public void testMakeEmpty() {
		GenericBinaryHeap<Double, Double> heap = new GenericBinaryHeap<Double, Double>();
		heap.makeEmpty();
		
		assertTrue("makeEmpty breaks empty heap", heap.isEmpty());
		
		heap.insert(50.0, 50.0);
		heap.makeEmpty();
		assertTrue("makeEmpty does not work for small heap", heap.isEmpty());
		
		addElements(heap, LARGE);
		heap.makeEmpty();
		assertTrue("makeEmpty does not work for large heap", heap.isEmpty());
	}
	
	private void addElements(GenericBinaryHeap<Double, Double> heap, int numElements) {
		addMinElements(heap, numElements, 0.0);
	}
	
	private void addMinElements(GenericBinaryHeap<Double, Double> heap, int numElements, double min) {
		Random r = new Random();
		for (int i = numElements; i > 0; --i) {
			double d = Math.max(min,r.nextInt(100));
			heap.insert(d, d);
		}
	}
	
	private void deleteElements(GenericBinaryHeap<Double, Double> heap, int numElements) {
		for (int i = numElements; i > 0; --i) {
			heap.deleteMin();
		}
	}


}
