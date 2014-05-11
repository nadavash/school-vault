package PriorityQueues;

/**
*
* @author Nadav Ashkenazi
* @date April 30, 2014
* @email nadava@uw.edu
* @studentID 1230523
* @version CSE373 14sp
* The BinaryHeap class represents a priority queue that simulates
* a binary tree with an array. This provides O(log(n)) deletion and
* insertion time into the heap.
*/
public class BinaryHeap implements PriorityQueue {

	public static final int STARTING_SIZE = 10;
	public static final int GROWTH_FACTOR = 2;
	public static final int ROOT_POSITION = 1;
	
	private double[] heapElements;
	private int size;
	
	/**
	 * Creates a new binary heap.
	 */
	public BinaryHeap() {
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
	// Each value lower in priority above it get moved down until no such
	// values exist.
	private int percolateUp(int hole, double value) {
		// Check if the any previous nodes have values higher
		// the the values we are entering
		while (hole > 1 && value < heapElements[hole / 2]) {
			// Swap the values and move up the tree
			heapElements[hole] = heapElements[hole / 2];
			hole /= 2;
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
	
	// Percolates down the tree until an element of bigger value is found.
	private int percolateDown(int hole, double value) {
		int left;
		int right;
		int target;
		while (2 * hole <= size) {
			// get the left and right children
			left = 2 * hole;
			right = left + 1;
			
			if (right > size || heapElements[left] < heapElements[right])
				target = left;
			else
				target = right;
			
			// Swap elements if the target is lower than the value
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
}