package PriorityQueues;

/**
 *
 * @author Nadav Ashkenazi
 * @date April 30, 2014
 * @email nadava@uw.edu
 * @studentID 1230523
 * @version CSE373 14sp
 * The GenericBinaryHeap class represents a priority queue that simulates
 * a binary tree with an array. This provides O(log(n)) deletion and
 * insertion time into the heap.
 * @param <T> The value type
 * @param <U> The comparator type (implements Comparabe<U>)
 */
public class GenericBinaryHeap<T, U extends Comparable<U>>{
	
	public static final int STARTING_SIZE = 10;
	public static final int GROWTH_FACTOR = 2;
	public static final int ROOT_POSITION = 1;
	
	private HeapNode<T, U>[] heapElements;
	private int size;
	
	/**
	 * Creates a new GenericBinaryHeap.
	 */
	public GenericBinaryHeap() {
		heapElements = new HeapNode[STARTING_SIZE];
	}
	
	/**
	 * Checks if the heap is empty.
	 * @return True if the heap is empty, false otherwise.
	 */
	public boolean isEmpty() {
		return size == 0;
	}

	/**
	 * Returns the size of the binary heap.
	 */
	public int size() {
		return size;
	}

	/**
	 * Finds the minimum element in the priority queue.
	 * @return The value of the minimum element.
	 * @throw EmptyPQException
	 * 		If the heap is empty
	 */
	public T findMin() {
		if (isEmpty())
			throw new EmptyPQException();
		
		return heapElements[ROOT_POSITION].value;
	}

	/**
	 * Inserts the specified value with the comparator in the binary heap.
	 * @param x The value to insert
	 * @param cmp The comparator for the value
	 */
	public void insert(T x, U cmp) {
		if (size == heapElements.length - 1)
			heapElements = copyOf(heapElements, heapElements.length * GROWTH_FACTOR);
		
		++size;
		int pos = percolateUp(size, cmp);
		heapElements[pos] = new HeapNode<T, U>(x, cmp);
	}
	
	// Percolates a specific value up from the specified hole position.
	// Each value lower in priority above it get moved down until no such
	// values exist.
	private int percolateUp(int hole, U cmp) {
		while (hole > 1 && cmp.compareTo(heapElements[hole / 2].comparator) < 0) {
			heapElements[hole] = heapElements[hole / 2];
			hole /= 2;
		}
		return hole;
	}

	/**
	 * Deletes the minimum element in the heap.
	 * @return The value of the minimum element.
	 * @throws EmprtyPQException
	 * 		If the heap is empty
	 */
	public T deleteMin() {
		if (isEmpty())
			throw new EmptyPQException();
		
		T res = heapElements[ROOT_POSITION].value;
		int hole = percolateDown(ROOT_POSITION, heapElements[size].comparator);
		heapElements[hole] = heapElements[size];
		heapElements[size] = null;
		size--;
		return res;
	}
	
	// Percolates down the tree until an element of bigger value is found.
	private int percolateDown(int hole, U cmp) {
		int left;
		int right;
		int target;
		while (2 * hole <= size) {
			left = 2 * hole;
			right = left + 1;
			
			target = right;
			if (right > size) {
				target = left;
			} else {
				U leftCmp = heapElements[left].comparator;
				U rightCmp = heapElements[right].comparator;
				
				if (leftCmp.compareTo(rightCmp) < 0)
					target = left;
			}
			
			if (heapElements[target].comparator.compareTo(cmp) < 0) {
				heapElements[hole] = heapElements[target];
				hole = target;
			} else
				break;
		}
		return hole;
	}

	/**
	 * Empties the list.
	 */
	public void makeEmpty() {
		size = 0;
	}
	
	// Copies an array of heap nodes into a new array
	private HeapNode<T, U>[] copyOf(HeapNode<T, U>[] original, int newLength) {
		HeapNode<T, U>[] copy = new HeapNode[newLength];
		
		for (int i = original.length - 1; i > 0; --i) {
			copy[i] = original[i];
		}
		
		return copy;
	}
	
	/**
	 * @author nadavash
	 * Represents a node in the generic heap.
	 * @param <T>
	 * @param <U>
	 */
	private static class HeapNode<T,U extends Comparable<U>> {
		public T value;
		public U comparator;
		
		public HeapNode(T value, U comparator) {
			this.value = value;
			this.comparator = comparator;
		}
	}
}
