import java.util.Arrays;

/**
 * 
 */

/**
 * @author nadavash
 *
 */
public class ThreeHeap implements PriorityQueue {

	public static final int STARTING_SIZE = 10;
	public static final int GROWTH_FACTOR = 2;
	public static final int ROOT_POSITION = 1;
	
	private double[] heapElements;
	private int size;
	
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
			resize();
		
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
	
	private int percolateDown(int hole, double value) {
		int left, right, middle, target;

		while (3 * hole - 1 <= size) {
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
	
	// Resizes heap and copies all values into a new heap with double the size.
	private void resize() {
		double[] copy = new double[heapElements.length * GROWTH_FACTOR];
		for (int i = size; i >= ROOT_POSITION; --i) {
			copy[i] = heapElements[i];
		}
		heapElements = copy;
	}
	
	// Gets the minimum value index for the provided index range
	private int getMin(int low, int high) {
		int target = low;
		for (int i = low + 1; i <= high; ++i) {
			if (heapElements[i] < heapElements[target])
				target = i;
		}
		return target;
	}
	
	
	public void printList() {
		System.out.println(Arrays.toString(heapElements));
	}

}
