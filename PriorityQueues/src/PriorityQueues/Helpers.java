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
}
