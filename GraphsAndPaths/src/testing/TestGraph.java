package testing;

import static org.junit.Assert.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

import graphs.*;

import org.junit.Before;
import org.junit.Test;

/**
 * TODO
 * @author nadavash
 *
 */
public class TestGraph {
	public static final String VERTEX_FILE = "vertex.txt";
	public static final String EDGE_FILE = "edge.txt";
	
	private MyGraph graph;
	private Collection<Vertex> vertices;
	private Collection<Edge> edges;
	

	@Before
	public void setUp() throws Exception {
		// Initialize myGraph
		Scanner s = null;
		try {
			s = new Scanner(new File(VERTEX_FILE));
		} catch(FileNotFoundException e1) {
			System.err.println("FILE NOT FOUND: "+VERTEX_FILE);
			System.exit(2);
		}

		vertices = new ArrayList<Vertex>();
		while(s.hasNext())
			vertices.add(new Vertex(s.next()));

		try {
			s = new Scanner(new File(EDGE_FILE));
		} catch(FileNotFoundException e1) {
			System.err.println("FILE NOT FOUND: "+EDGE_FILE);
			System.exit(2);
		}

		edges = new ArrayList<Edge>();
		while(s.hasNext()) {
			try {
				Vertex a = new Vertex(s.next());
				Vertex b = new Vertex(s.next());
				int w = s.nextInt();
				edges.add(new Edge(a,b,w));
			} catch (NoSuchElementException e2) {
				System.err.println("EDGE FILE FORMAT INCORRECT");
				System.exit(3);
			}
		}

		graph = new MyGraph(vertices, edges);
	}

	@Test
	public void testMyGraph() {
		fail("Not yet implemented");
	}

	@Test
	public void testVertices() {
		Collection<Vertex> vs = graph.vertices();
		
		// Test all vertices from file are in the list
		for (Vertex v : vertices) {
			assertTrue("Graph vertices do not contain all necessary vertices.", vs.contains(v));
		}
	}

	@Test
	public void testEdges() {
		Collection<Edge> es = graph.edges();
		
		// Test all edges from file are in the list
		for (Edge e : edges) {
			assertTrue("Graph edges do not contain all necessary edges.", es.contains(e));
		}
	}

	@Test
	public void testAdjacentVertices() {
		fail("Not yet implemented");
	}

	@Test
	public void testEdgeCost() {
		for (Edge e : edges) {
			int cost = graph.edgeCost(e.getSource(), e.getDestination());
			assertTrue("EdgeCost not calcualated correctly.", cost == e.getWeight());
		}
	}

	@Test
	public void testShortestPath() {
		fail("Not yet implemented");
	}

}
