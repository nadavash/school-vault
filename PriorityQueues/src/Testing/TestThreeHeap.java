package Testing;
import PriorityQueues.PriorityQueue;
import PriorityQueues.ThreeHeap;


/**
 * 
 */

/**
 * @author nadavash
 *
 */
public class TestThreeHeap extends TestPriorityQueue {
	
	public PriorityQueue createQueue() {
		return new ThreeHeap();
	}
	
}
