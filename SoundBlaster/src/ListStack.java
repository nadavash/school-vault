import java.util.EmptyStackException;

/**
 * 
 * @author nadavash
 * The ListStack class represents a stack that uses a linked list
 * as its storage mechanism. This provides for a constant time push
 * and pop operations. This stack acts as a LIFO structure that holds
 * double values.
 */
public class ListStack implements DStack {

	private ListStackNode top;
	
	@Override
	public boolean isEmpty() {
		return top == null;
	}

	@Override
	public void push(double d) {
		top = new ListStackNode(d, top);
	}

	@Override
	public double pop() {
		if (top == null)
			throw new EmptyStackException();
		
		double val = top.value;
		top = top.next;
		return val;
	}

	@Override
	public double peek() {
		if (top == null)
			throw new EmptyStackException();
		
		return top.value;
	}

	/**
	 * The ListStackNode represents one node in the ListStack
	 * linked list.
	 */
	public static class ListStackNode {
		public double value;
		public ListStackNode next;
		
		public ListStackNode(double value, ListStackNode next) {
			this.value = value;
			this.next = next;
		}
	}
}
