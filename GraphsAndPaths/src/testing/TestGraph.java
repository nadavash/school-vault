package testing;

import static org.junit.Assert.*;

import java.io.File;
import java.util.*;

import graphs.*;

import org.junit.Before;
import org.junit.Test;

public class TestGraph {
	public static final String VERTEX_FILE = "vertex.txt";
	public static final String EDGE_FILE = "edge.txt";
	
	private MyGraph graph;

	@Before
	public void setUp() throws Exception {
		// Initialize myGraph
		graph = FindPaths.readGraph(VERTEX_FILE, EDGE_FILE);
	}

	@Test
	public void testMyGraph() {
		fail("Not yet implemented");
	}

	@Test
	public void testVertices() {
		fail("Not yet implemented");
	}

	@Test
	public void testEdges() {
		fail("Not yet implemented");
	}

	@Test
	public void testAdjacentVertices() {
		fail("Not yet implemented");
	}

	@Test
	public void testEdgeCost() {
		fail("Not yet implemented");
	}

	@Test
	public void testShortestPath() {
		fail("Not yet implemented");
	}

}
