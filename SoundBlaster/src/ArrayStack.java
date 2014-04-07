import java.util.*;

/**
 *
 * @author nadavash
 * The ArrayStack class represents a LIFO stack of doubles.
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

	@Override
	public boolean isEmpty() {
		return size == 0;
	}

	@Override
	public void push(double d) {
		// Resize the array to 2 times its previous size if it is full
		if (size == data.length) {
			data = Arrays.copyOf(data, size * 2);
		}
		
		data[size++] = d;
	}

	@Override
	public double pop() {
		if (isEmpty())
			throw new EmptyStackException();
		
		size--;
		
		// Resize the stack if it is 3/4 empty (extra credit)
		if (size * 4 <= data.length)
			data = Arrays.copyOf(data, data.length / 2);
		
		return data[size];
	}

	@Override
	public double peek() {
		if (isEmpty())
			throw new EmptyStackException();
		
		return data[size - 1];
	}
	
}
