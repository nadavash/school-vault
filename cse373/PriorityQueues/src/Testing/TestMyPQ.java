package Testing;
import PriorityQueues.MyPQ;
import PriorityQueues.PriorityQueue;


public class TestMyPQ extends TestPriorityQueue {
	
	public PriorityQueue createQueue() {
		return new MyPQ();
	}

}
