/**
 * 
 */

/**
 * @author nadavash
 *
 */
public class MyPQ implements PriorityQueue {
	private ListNode head;
	private int size;

	@Override
	public boolean isEmpty() {
		// TODO Auto-generated method stub
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
