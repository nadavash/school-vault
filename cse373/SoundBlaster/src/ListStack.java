import java.util.EmptyStackException;

/**
 * 
 * @author Nadav Ashkenazi
 * @date April 9, 2014
 * @email nadava@uw.edu
 * @studentID 1230523
 * @version CSE373 14sp
 * The ListStack class represents a stack that uses a linked list
 * as its storage mechanism. This provides for a constant time push
 * and pop operations. This stack acts as a LIFO structure that holds
 * double values.
 */
public class ListStack implements DStack {

	private ListStackNode top;
	
	/**
	 * Tests if the stack has no elements;
	 * @return true if the are no elements in the stack, and returns
	 * false otherwise.
	 */
	@Override
	public boolean isEmpty() {
		return top == null;
	}

	/**
	 * Pushes an element on top of the stack;
	 * @param d
	 * 		The value to push onto the stack.
	 */
	@Override
	public void push(double d) {
		top = new ListStackNode(d, top);
	}

	/**
	 * Pops an element out of the stack.
	 * @return
	 * 		The value that was popped of the stack.
	 * @throws EmptyStackException
	 * 		If the stack is empty
	 */
	public double pop() {
		if (top == null)
			throw new EmptyStackException();
		
		double val = top.value;
		top = top.next;
		return val;
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
		
		/**
		 * Creates a new list node.
		 * @param value
		 * 		The value to give to the list node.
		 * @param next
		 * 		The next node in the list.
		 */
		public ListStackNode(double value, ListStackNode next) {
			this.value = value;
			this.next = next;
		}
	}
}
