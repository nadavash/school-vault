import java.util.EmptyStackException;

/**
 * 
 * @author Nadav Ashkenazi
 * @date April 9, 2014
 * @email nadava@uw.edu
 * @studentID 1230523
 * @version CSE373 14sp
 * 
 *          Question 4. Suppose that, instead of a DStack interface, you were
 *          given a fully-functional FIFO Queue class. How might you implement
 *          this project (i.e., simulate a Stack) with one or more instances of
 *          a FIFO Queue?
 * 
 *          The FIFOQueue class provides the operations enqueue, dequeue,
 *          isEmpty and size and those are the ONLY operations that you can use.
 */
public class QueueStack implements DStack {
    // The queue object
    private FIFOQueue queue;

    // you can add more queue instances here and initialize in the constructor
    private FIFOQueue queue2;

    /**
     * Constructor. Initialize the instance field
     */
    public QueueStack() {
        queue = new FIFOQueue();
        // initialize extra queue object if necessary
        queue2 = new FIFOQueue();
    }

    /**
     * Pushes the value onto the stack represented by this queue. In other
     * words, inserts the element at the front of this queue.
     * 
     * @param value
     *            the value that push into the stack
     */
    @Override
    public void push(double value) {
        // TODO write your push operation here
    	queue.enqueue(value);
    }

    /**
     * Pops the value from the stack represented by this queue. In other words,
     * removes and returns the first element of this queue.
     * 
     * @return the value that at the end of the stack
     */
    @Override
    public double pop() {
    	if (queue.isEmpty()) {
    		throw new EmptyStackException();
    	}
    	
    	while (queue.size() > 1) {
    		queue2.enqueue(queue.dequeue());
    	}
    	
    	double val = queue.dequeue();
    	FIFOQueue temp = queue;
    	queue = queue2;
    	queue2 = temp;
    	
    	return val;
    }

    // ////////////////////////////////////////////////////////////////
    // Ignore all the methods below, those are only for testing
    // ////////////////////////////////////////////////////////////////

    /**
     * Check if the queue is empty
     * 
     * @return true if the queue is empty, false otherwise
     */
    @Override
    public boolean isEmpty() {
        return queue.isEmpty();
    }

    /**
     * Peek the last element
     * 
     * @return the last element in the queue object
     */
    @Override
    public double peek() {
        return queue.peekLast();
    }
}
