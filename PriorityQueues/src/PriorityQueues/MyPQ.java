package PriorityQueues;
/**
 * 
 */

/**
*
* @author Nadav Ashkenazi
* @date April 30, 2014
* @email nadava@uw.edu
* @studentID 1230523
* @version CSE373 14sp
* The MyPQ class represents a priority queue that makes use of a linked
* list for . This provides O(n) deletion time and O(1) insertion time into 
* the queue.
*/
public class MyPQ implements PriorityQueue {
	
	private ListNode head;
	private int size;

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
		
		return head.value;
	}

	@Override
	public void insert(double x) {
		size++;
		
		if (head == null) {
			head = new ListNode(x);
			return;
		}
		
		if (head.value >= x) {
			head = new ListNode(x, head);
			return;
		}
		
		ListNode current = head;
		while (current.next != null) {
			if (current.next.value >= x) {
				current.next = new ListNode(x, current.next);
				return;
			}
			current = current.next;
		}
		
		current.next = new ListNode(x);
	}

	@Override
	public double deleteMin() {
		if (isEmpty())
			throw new EmptyPQException();
		
		double deleted = head.value;
		head = head.next;
		size--;
		return deleted;
	}

	@Override
	public void makeEmpty() {
		head = null;
		size = 0;
	}

	/**
	 * Represents a node in the linked list for the MyPQ priority queue.
	 */
	private static class ListNode {
		public double value;
		public ListNode next;
		
		public ListNode(double value, ListNode next) {
			this.value = value;
			this.next = next;
		}
		
		public ListNode(double value) {
			this(value, null);
		}
	}
}
