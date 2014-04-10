

import static org.junit.Assert.*;

import java.util.EmptyStackException;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class ArrayStackTest {

	@Test
	public void testPush() {
		ArrayStack stack = new ArrayStack();
		stack.push(50);
		stack.push(40);
		stack.push(30);
		
		stack = CreateFilledStack(1000000);
	}

	@Test
	public void testPop() {
		ArrayStack stack = new ArrayStack();
		stack.push(50);
		stack.push(40);
		stack.push(30);
		
		System.out.println(stack.pop());
		//assertEquals("Pop does not pop the correct element.", 30.0, stack.pop(), 0.2);
		assertEquals("Pop does not pop the correct element.", 40.0, stack.pop(), 0.2);
		assertEquals("Pop does not pop the correct element.", 50.0, stack.pop(), 0.2);
		assertTrue("Stack isn't empty.", stack.isEmpty());
		
		try {
			stack.pop();
			fail("Empty stack exception not thrown.");
		} catch (EmptyStackException e) { }
	}

	@Test
	public void testPeek() {
		ArrayStack stack = new ArrayStack();
		try {
			stack.peek();
			fail("Empty stack exception not thrown.");
		} catch (EmptyStackException e) { }
		
		stack.push(50);
		stack.push(40);
		stack.push(30);
		
		assertEquals("Peak does not return correct value.", 30.0, stack.peek(), 0.2);
		stack.pop();
		assertEquals("Peak does not return correct value.", 40.0, stack.peek(), 0.2);
	}
	
	@Test
	public void testIsEmpty() {
		ArrayStack stack = new ArrayStack();
		assertTrue("Stack isn't empty", stack.isEmpty());
		
		stack.push(302.2);
		assertFalse("Stack is empty", stack.isEmpty());
		
		stack.pop();
		assertTrue("Stack isn't empty", stack.isEmpty());
	}
	
	private ArrayStack CreateFilledStack(int num) {
		ArrayStack stack = new ArrayStack();
		
		for (int i = 0; i < num; ++i) {
			stack.push(Math.random() * 50);
		}
		
		return stack;
	}

}