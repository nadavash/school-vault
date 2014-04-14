import java.util.*;

/**
 *
 * @author Nadav Ashkenazi
 * @date April 9, 2014
 * @email nadava@uw.edu
 * @studentID 1230523
 * @version CSE373 14sp
 * The ArrayStack class represents a LIFO stack of doubles. The
 * stack starts at a fixed size and grows as needed. When the
 * stack expands, the push operation will have a time complexity of
 * O(n), and that applies also to the pop operation when the
 * stack compresses.
 */
public class ArrayStack implements DStack {
	private double[] data;
	private int size;
	
	/*
	 * Creates a new array stack.
	 */
	public ArrayStack() {
		size = 0;
		data = new double[10];
	}

	/**
	 * Tests if the stack has no elements;
	 * @return true if the are no elements in the stack, and returns
	 * false otherwise.
	 */
	@Override
	public boolean isEmpty() {
		return size == 0;
	}

	
	/**
	 * Pushes an element on top of the stack;
	 * @param d
	 * 		The value to push onto the stack.
	 */
	@Override
	public void push(double d) {
		// Resize the array to 2 times its previous size if it is full
		if (size == data.length) {
			data = copyOf(data, size * 2);
		}
		
		data[size++] = d;
	}

	/**
	 * Pops an element out of the stack.
	 * @return
	 * 		The value that was popped of the stack.
	 * @throws EmptyStackException
	 * 		If the stack is empty
	 */
	@Override
	public double pop() {
		if (isEmpty())
			throw new EmptyStackException();
		
		size--;
		
		// Resize the stack if it is 3/4 empty (extra credit)
		if (size * 4 <= data.length && size > 10)
			data = copyOf(data, data.length / 2);
		
		return data[size];
	}

	/**
	 * Look at top value of the stack.
	 * @return
	 * 		The value of the stack
	 * @throws EmptyStackException
	 * 		if the stack is empty
	 */
	@Override
	public double peek() {
		if (isEmpty())
			throw new EmptyStackException();
		
		return data[size - 1];
	}
	
	// Copies all values into a new array with a specified length.
	private double[] copyOf(double[] original, int newLength) {
		double[] copy = new double[newLength];
		
		for (int i = size - 1; i >= 0; --i) {
			copy[i] = original[i];
		}
		
		return copy;
	}
}
