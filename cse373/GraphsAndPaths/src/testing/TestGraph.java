package testing;

import static org.junit.Assert.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

import graphs.*;

import org.junit.Before;
import org.junit.Test;

/**
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 * The TestGraph class tests the MyGraph class with sample text vertex
 * and edge files designed specifically for the test cases here.
 */
public class TestGraph {
	public static final String VERTEX_FILE = "test-vertex1.txt";
	public static final String EDGE_FILE = "test-edge1.txt";
	
	private MyGraph graph;
	private List<Vertex> vertices;
	private List<Edge> edges;
	
	// Small edge case vertices
	private List<Vertex> smVertices = new ArrayList<Vertex>();
	private List<Edge> smEdges = new ArrayList<Edge>();

	@Before
	public void setUp() throws Exception {
		// Initialize small vertices and edges lists
		smVertices = new ArrayList<Vertex>();
		smEdges = new ArrayList<Edge>();
		smVertices.add(new Vertex("A"));
		smVertices.add(new Vertex("B"));
		
		smEdges.add(new Edge(smVertices.get(0),smVertices.get(1),1));
		smEdges.add(new Edge(smVertices.get(1),smVertices.get(0),1));
		
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
		// Test basic functionality of constructor
		
		// Test vertices and edges
		
		smEdges.set(1, new Edge(smVertices.get(1), smVertices.get(0),-4));
		
		// Test NegativeWeightException
		try {
			new MyGraph(smVertices, smEdges);
			fail("Constructor did not throw exception for edge with negative weight");
		} catch (NegativeWeightException e) { }
		
		// Test InvalidVertexException
		smEdges.set(1, new Edge(smVertices.get(0), new Vertex("yolo"), 3));
		
		try {
			new MyGraph(smVertices, smEdges);
			fail("Constructor did not throw exception for nonexistant vertex");
		} catch (InvalidVertexException e) { }
		
		// Test InvalidDuplicateEdgeException
		smEdges.set(1, new Edge(smVertices.get(0), smVertices.get(1), 44));
		
		try {
			new MyGraph(smVertices, smEdges);
			fail("Constructor did not throw exception for invalid duplicate edge");
		} catch (InvalidDuplicateEdgeException e) { }
	}

	@Test
	public void testVertices() {
		Collection<Vertex> vs = graph.vertices();
		
		// Test all vertices from file are in the list
		for (Vertex v : vertices) {
			assertTrue("Graph vertices do not contain all necessary vertices", vs.contains(v));
		}
	}

	@Test
	public void testEdges() {
		Collection<Edge> es = graph.edges();
		
		// Test all edges from file are in the list
		for (Edge e : edges) {
			assertTrue("Graph edges do not contain all necessary edges", es.contains(e));
		}
	}

	@Test
	public void testAdjacentVertices() {
		// no adjacent vertices
		Collection<Vertex> vertices = graph.adjacentVertices(new Vertex("D"));
		assertTrue("Adjacent vertex list should be zero for vertex with no adjacents", 
				vertices.isEmpty());
		
		// some adjacent vertices
		vertices = graph.adjacentVertices(new Vertex("A"));
		assertTrue("Adjacent vertex list should be of size 2 for vertex with two adjacents", 
				vertices.size() == 2);
		assertTrue("List does not contain one of the neighbors", vertices.contains(new Vertex("B")));
		assertTrue("List does not contain one of the neighbors", vertices.contains(new Vertex("C")));
		
		// Test exception
		try {
			graph.adjacentVertices(new Vertex("some nonexistant vertex"));
			fail("Adjacent vertices method did not throw exception for nonexistant vertex");
		} catch(IllegalArgumentException e) { }
	}

	@Test
	public void testEdgeCost() {
		// Test normal cases
		for (Edge e : edges) {
			int cost = graph.edgeCost(e.getSource(), e.getDestination());
			assertTrue("EdgeCost not calcualated correctly", cost == e.getWeight());
		}
		
		// Test exception
		try {
			graph.edgeCost(new Vertex("Yolo"), new Vertex("Yolo more"));
			fail("Edge cost does not throw exception for nonexistant vertices");
		} catch (IllegalArgumentException e) { }
	}

	@Test
	public void testShortestPath() {		
		smEdges.set(1, new Edge(smVertices.get(1),smVertices.get(0),1));

		MyGraph graphic = new MyGraph(smVertices, smEdges);
		
		// Test very short path
		Path p = graphic.shortestPath(smVertices.get(0), smVertices.get(1));
		assertTrue("Cost of the returned path is incorrect", p.cost == 1);
		
		// Test no path
		p = graph.shortestPath(new Vertex("B"), new Vertex("A"));
		assertTrue("Shortest path should return null for no path", p == null);
		
		// Test path as one vertex (0 weight)
		p = graph.shortestPath(new Vertex("A"), new Vertex("A"));
		assertTrue("Path cost of a == b should be zero", p.cost == 0);
		assertTrue("Path length should be one vertex long", p.vertices.size() == 1);
		assertTrue("Path should only contain vertex A", p.vertices.contains(new Vertex("A")));
		
		// Test regular path
		p = graph.shortestPath(new Vertex("A"), new Vertex("D"));
		assertTrue("Path cost of regular path is incorrect", p.cost == 7);
		assertTrue("Regular path does not have the correct number of vertices", p.vertices.size() == 3);
		
		// Test exception
		try {
			graph.shortestPath(new Vertex("some vertex that does not exist"),
					new Vertex("some other vertex that doesn't exist"));
			fail("Shortest path method does not throw exception when vertices "
					+ "aren't in the graph");
		} catch(IllegalArgumentException e) { }
	}

}
