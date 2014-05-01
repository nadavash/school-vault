package PriorityQueues;

/**
*
* @author Nadav Ashkenazi
* @date April 30, 2014
* @email nadava@uw.edu
* @studentID 1230523
* @version CSE373 14sp
* The ThreeHeap class represents a priority queue that simulates
* a ternary tree with an array. This provides O(log3(n)) deletion and
* insertion time into the heap.
*/
public class ThreeHeap implements PriorityQueue {

	public static final int STARTING_SIZE = 10;
	public static final int GROWTH_FACTOR = 2;
	public static final int ROOT_POSITION = 1;
	
	private double[] heapElements;
	private int size;
	
	/**
	 * Creates a new ThreeHeap.
	 */
	public ThreeHeap() {
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
		while (hole > 1 && value < heapElements[(hole + 1) / 3]) {
			heapElements[hole] = heapElements[(hole + 1) / 3];
			hole = (hole + 1) / 3;
		}
		return hole;
	}

	@Override
	public double deleteMin() {
		if (isEmpty())
			throw new EmptyPQException();
		
		double res = heapElements[ROOT_POSITION];
		int hole = percolateDown(1, heapElements[size]);
		heapElements[hole] = heapElements[size]; 
		size--;
		
		return res;
	}
	
	// Percolates down the tree until an element of bigger value is found.
	private int percolateDown(int hole, double value) {
		int left, right, middle, target;

		while (3 * hole - 1 <= size) {
			// Get all children indices
			middle = hole * 3;
			left = middle - 1;
			right = middle + 1;
			
			if (middle > size)
				target = left;
			else if (right > size)
				target = getMin(left, middle);
			else
				target = getMin(left, right);
			
			if (heapElements[target] < value) {
				heapElements[hole] = heapElements[target];
				hole = target;
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
}
