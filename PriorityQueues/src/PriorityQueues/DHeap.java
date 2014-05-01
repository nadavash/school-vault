package PriorityQueues;

/**
 * 
 * @author nadavash
 *
 */
public class DHeap implements PriorityQueue {
	
	public static final int STARTING_SIZE = 10;
	public static final int GROWTH_FACTOR = 2;
	public static final int ROOT_POSITION = 1;
	public static final int DEFAULT_DIM = 2;
	
	private double[] heapElements;
	private int size;
	private int dim;
	
	public DHeap() {
		this(DEFAULT_DIM);
	}
	
	public DHeap(int d) {
		dim = d;
		heapElements = new double[STARTING_SIZE];
	}
	
	@Override
	public boolean isEmpty() {
		return size == 0;
	}

	@Override
	public int size() {
		return size;
	}

	@Override
	public double findMin() {
		if (isEmpty())
			throw new EmptyPQException();
		
		return heapElements[ROOT_POSITION];
	}

	@Override
	public void insert(double x) {
		if (size == heapElements.length - 1)
			heapElements = Helpers.copyOf(heapElements, 
					heapElements.length * GROWTH_FACTOR);
		
		++size;
		int pos = percolateUp(size, x);
		heapElements[pos] = x;
	}
	
	// Percolates a specific value up from the specified hole position.
	// Each value lower in priority above it get move down until no such
	// values exist.
	private int percolateUp(int hole, double value) {
		while (hole > 1 && 
				value < heapElements[getParentIndex(hole, dim)]) {
			heapElements[hole] = heapElements[getParentIndex(hole, dim)];
			hole = getParentIndex(hole, dim);
		}
		
		return hole;
	} 

	@Override
	public double deleteMin() {
		if (isEmpty())
			throw new EmptyPQException();
		
		double res = heapElements[ROOT_POSITION];
		int hole = percolateDown(ROOT_POSITION, heapElements[size]);
		heapElements[hole] = heapElements[size];
		size--;
		
		return res;
	}
	
	private int percolateDown(int hole, double value) {
		int firstChild = getFirstChildIndex(hole, dim);
		
		while (firstChild <= size) {
			int target = getMin(firstChild, firstChild + dim - 1);
			
			if (heapElements[target] < value) {
				heapElements[hole] = heapElements[target];
				hole = target;
				firstChild = getFirstChildIndex(hole, dim);
			} else
				break;
		}
		
		return hole;
	}

	@Override
	public void makeEmpty() {
		size = 0;
	}
	
	// Gets the minimum value index for the provided index range
	private int getMin(int low, int high) {
		int target = low;
		for (int i = low + 1; i <= size && i <= high; ++i) {
			if (heapElements[i] < heapElements[target])
				target = i;
		}
		return target;
	}
	
	/**
	 * Calculates the parent node index in a d-heap array.
	 * @param i The index of the child node.
	 * @param dim The dimension of the d-heap.
	 * @return The index of the parent node.
	 */
	private int getParentIndex(int i, int dim) {
		return (i + dim - 2) / dim;
	}
	
	/**
	 * Calculates the first child node index in a d-heap array.
	 * @param i The index of the child node.
	 * @param dim The dimension of the d-heap.
	 * @return The index of the first child node.
	 */
	private int getFirstChildIndex(int i, int dim) {
		return i * dim - (dim - 2);
	}
}
