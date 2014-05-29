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
			fail("Constructor did not throw exception for nonexistant vertex.");
		} catch (InvalidVertexException e) { }
		
		// Test InvalidDuplicateEdgeException
		smEdges.set(1, new Edge(smVertices.get(0), smVertices.get(1), 44));
		
		try {
			new MyGraph(smVertices, smEdges);
			fail("Constructor did not throw exception for invalid duplicate edge.");
		} catch (InvalidDuplicateEdgeException e) { }
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
		smEdges.set(1, new Edge(smVertices.get(1),smVertices.get(0),1));

		MyGraph graphic = new MyGraph(smVertices, smEdges);
		Path p = graphic.shortestPath(smVertices.get(0), smVertices.get(1));
		
		assertTrue("Cost of the returned path is incorrect.", p.cost == 1);
		//assertTrue()
		
		// TODO: Test no path
		p = graph.shortestPath(vertices.get(0), vertices.get(vertices.size() - 1));
		
		// TODO: Test path as one vertex (0 weight)
		
		// TODO: Test regular path
		p = graph.shortestPath(new Vertex("A"), new Vertex("D"));
		assertTrue("Path cost of regular path is incorrect", p.cost == 7);
		assertTrue("Regular path does not have the correct number of vertices.", p.vertices.size() == 3);
		
		// Test exception
		try {
			graph.shortestPath(new Vertex("some vertex that does not exist"),
					new Vertex("some other vertex that doesn't exist"));
			fail("Shortest path method does not throw exception when vertices "
					+ "aren't in the graph");
		} catch(IllegalArgumentException e) { }
	}

}
