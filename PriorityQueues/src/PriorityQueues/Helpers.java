package PriorityQueues;
/**
 * 
 * @author nadavash
 * Provides a few static helper methods for the PriorityQueue assignment.
 */
public final class Helpers {
	
	// Private constructor to prevent instantiation of 'static' class
	private Helpers() { }
	
	/**
	 * Copies all values from the provided array into a new array.
	 * @param original the array to copy values from
	 * @param newLength the length of the new array to return
	 * @return a new array with the copied values
	 */
	public static double[] copyOf(double[] original, int newLength) {
		double[] copy = new double[newLength];
		
		for (int i = original.length - 1; i >= 0; --i) {
			copy[i] = original[i];
		}
		
		return copy;
	}
	
	/**
	 * Calculates the parent node index in a d-heap array.
	 * @param i The index of the child node.
	 * @param dim The dimension of the d-heap.
	 * @return The index of the parent node.
	 */
	public static int getParentIndex(int i, int dim) {
		return (i + dim - 2) / dim;
	}
	
	/**
	 * Calculates the first child node index in a d-heap array.
	 * @param i The index of the child node.
	 * @param dim The dimension of the d-heap.
	 * @return The index of the first child node.
	 */
	public static int getFirstChildIndex(int i, int dim) {
		return i * dim - (dim - 2);
	}
}
