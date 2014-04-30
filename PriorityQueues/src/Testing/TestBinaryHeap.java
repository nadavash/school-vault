package Testing;
import PriorityQueues.BinaryHeap;
import PriorityQueues.PriorityQueue;


/**
 * 
 */

/**
 * @author nadavash
 *
 */
public class TestBinaryHeap extends TestPriorityQueue {
	
	public PriorityQueue createQueue() {
		return new BinaryHeap();
	}
	
}
