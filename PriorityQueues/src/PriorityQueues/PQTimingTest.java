package PriorityQueues;
import java.util.Random;

/**
 * 
 */

/**
 * @author nadavash
 *
 */
public class PQTimingTest {
	
	public static final int[] LENGTHS = new int[] { 10000, 20000, 30000, 40000 };//, 1000000 };
	private static final PriorityQueue[] QUEUES = new PriorityQueue[] {
		new BinaryHeap(),
		new ThreeHeap(),
		new MyPQ()
	};
	
	private Random random = new Random();
	
	// Timing related variables
	private long time;
	
	public static void main(String[] args) {
		PQTimingTest testing = new PQTimingTest();
		for (PriorityQueue q : QUEUES) {
			testing.runTests(q);
		}
	}
	
	public void runTests(PriorityQueue queue) {
		System.out.print("Testing: ");
		System.out.print(queue.getClass());
		System.out.println();
		
		for (int length : LENGTHS) {
			timeFillUp(queue, length);
			System.out.print("\t");
			timeEmptying(queue, length);
			System.out.print("\t");
		}
		
		System.out.println();
	}
	
	private void timeFillUp(PriorityQueue q, int n) {
		startTimer();
		
		for (int i = n; i > 0; --i) {
			q.insert(random.nextDouble());
		}
		
		System.out.print(getTime());
	}
	
	private void timeEmptying(PriorityQueue q, int n) {
		startTimer();
		
		while (!q.isEmpty()) {
			q.deleteMin();
		}
		
		System.out.print(getTime());
	}

	private void startTimer() {
		time = System.nanoTime();
	}
	
	private long getTime() {
		return System.nanoTime() - time;
	}
}
