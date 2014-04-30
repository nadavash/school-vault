package PriorityQueues;

/**
 * 
 * @author nadavash
 *
 * @param <T>
 * @param <U>
 */
public class GenericBinaryHeap<T, U extends Comparable<U>>{
	
	public static final int STARTING_SIZE = 10;
	public static final int GROWTH_FACTOR = 2;
	public static final int ROOT_POSITION = 1;
	
	private HeapNode<T, U>[] heapElements;
	private int size;
	
	public GenericBinaryHeap() {
		heapElements = new HeapNode[STARTING_SIZE];
	}
	
	public boolean isEmpty() {
		return size == 0;
	}

	public int size() {
		return size;
	}

	public T findMin() {
		if (isEmpty())
			throw new EmptyPQException();
		
		return heapElements[ROOT_POSITION].value;
	}

	/**
	 * 
	 * @param x
	 * @param cmp
	 */
	public void insert(T x, U cmp) {
		if (size == heapElements.length - 1)
			heapElements = copyOf(heapElements, 
					heapElements.length * GROWTH_FACTOR);
		
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
	
	private int percolateDown(int hole, U cmp) {
		int left;
		int right;
		int target;
		while (2 * hole <= size) {
			left = 2 * hole;
			right = left + 1;
			
			U leftCmp = heapElements[left].comparator;
			U rightCmp = heapElements[right].comparator;
			if (right > size || leftCmp.compareTo(rightCmp) < 0)
				target = left;
			else
				target = right;
			
			if (heapElements[target].comparator.compareTo(cmp) < 0) {
				heapElements[hole] = heapElements[target];
				hole = target;
			} else
				break;
		}
		return hole;
	}

	public void makeEmpty() {
		size = 0;
	}
	
	private HeapNode<T, U>[] copyOf(HeapNode<T, U>[] original, int newLength) {
		HeapNode<T, U>[] copy = new HeapNode[newLength];
		
		for (int i = original.length - 1; i >= 0; --i) {
			copy[i] = original[i];
		}
		
		return copy;
	}
	
	private static class HeapNode<T,U extends Comparable<U>> {
		public T value;
		public U comparator;
		
		public HeapNode(T value, U comparator) {
			this.value = value;
			this.comparator = comparator;
		}
	}
}
