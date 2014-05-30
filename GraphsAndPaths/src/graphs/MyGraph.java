package graphs;

import java.util.*;

/**
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 * 
 * A representation of a graph. Assumes that we do not have negative cost edges
 * in the graph.
 */
public class MyGraph implements Graph {
	
	private Map<Vertex, List<Edge>> graph; 

	/**
	 * Creates a MyGraph object with the given collection of vertices and the
	 * given collection of edges.
	 * 
	 * @param v
	 *            a collection of the vertices in this graph
	 * @param e
	 *            a collection of the edges in this graph
	 */
	public MyGraph(Collection<Vertex> v, Collection<Edge> e) {
		graph = new HashMap<Vertex, List<Edge>>();
		
		// Add vertices to the graph
		for (Vertex vertex : v) {
			if (!graph.containsKey(vertex))
				graph.put(vertex, new LinkedList<Edge>());
		}
		
		// Add edges to the corresponding vertices on the graph
		for (Edge edge : e) {
			Vertex src = edge.getSource();
			Vertex destination = edge.getDestination();
			
			// Check that the weight is not negative
			if(edge.getWeight() < 0)
				throw new NegativeWeightException("Edge weights cannot be negative. ");
				
			// Check if the graph contains destination and source vertices
			if (!(graph.containsKey(src) && graph.containsKey(destination)))
				throw new InvalidVertexException("Edge contains invalid vertex .");
				
			// Check if direction already exists
			boolean foundDuplicate = false;
			for (Edge srcEdge : graph.get(src)) {
				if (srcEdge.getDestination().equals(destination)) {
					foundDuplicate = true;
					
					if (srcEdge.getWeight() != edge.getWeight())
						throw new InvalidDuplicateEdgeException
						("Duplicate edge bounds with different weights found.");
				}
			}

			if (!foundDuplicate)
				graph.get(src).add(edge);
		}
	}

	/**
	 * Return the collection of vertices of this graph
	 * 
	 * @return the vertices as a collection (which is anything iterable)
	 */
	@Override
	public Collection<Vertex> vertices() {
		return graph.keySet();
	}

	/**
	 * Return the collection of edges of this graph
	 * 
	 * @return the edges as a collection (which is anything iterable)
	 */
	@Override
	public Collection<Edge> edges() {
		List<Edge> edges = new LinkedList<Edge>();
		for (Vertex vertex : graph.keySet()) {
			for (Edge edge : graph.get(vertex)) {
				edges.add(edge);
			}
		}
		
		return edges;
	}

	/**
	 * Return a collection of vertices adjacent to a given vertex v. i.e., the
	 * set of all vertices w where edges v -> w exist in the graph. Return an
	 * empty collection if there are no adjacent vertices.
	 * 
	 * @param v
	 *            one of the vertices in the graph
	 * @return an iterable collection of vertices adjacent to v in the graph
	 * @throws IllegalArgumentException
	 *             if v does not exist.
	 */
	@Override
	public Collection<Vertex> adjacentVertices(Vertex v) {
		if (!graph.containsKey(v))
			throw new IllegalArgumentException();
		
		List<Vertex> adjacents = new LinkedList<Vertex>();
		for (Edge edge : graph.get(v)) {
			adjacents.add(edge.getDestination());
		}
		
		return adjacents;
	}

	/**
	 * Test whether vertex b is adjacent to vertex a (i.e. a -> b) in a directed
	 * graph. Assumes that we do not have negative cost edges in the graph.
	 * 
	 * @param a
	 *            one vertex
	 * @param b
	 *            another vertex
	 * @return cost of edge if there is a directed edge from a to b in the
	 *         graph, return -1 otherwise.
	 * @throws IllegalArgumentException
	 *             if a or b do not exist.
	 */
	@Override
	public int edgeCost(Vertex a, Vertex b) {
		if (!(graph.containsKey(a) && graph.containsKey(b)))
			throw new IllegalArgumentException();
		
		for(Edge e : graph.get(a)) {
			if(e.getDestination().equals(b)) {
				return e.getWeight();
			}
		}
		
		return -1;
	}

	/**
	 * Returns the shortest path from a to b in the graph, or null if there is
	 * no such path. Assumes all edge weights are nonnegative. Uses Dijkstra's
	 * algorithm.
	 * 
	 * @param a
	 *            the starting vertex
	 * @param b
	 *            the destination vertex
	 * @return a Path where the vertices indicate the path from a to b in order
	 *         and contains a (first) and b (last) and the cost is the cost of
	 *         the path. Returns null if b is not reachable from a.
	 * @throws IllegalArgumentException
	 *             if a or b does not exist.
	 */
	public Path shortestPath(Vertex a, Vertex b) {
		// If either one of the vertices are not in the graph, throw exception
		if (!(graph.containsKey(a) && graph.containsKey(b)))
			throw new IllegalArgumentException();
		
		// Initialize vertices and the unknown set
		Map<Vertex, Vertex> vertices = buildInitMap(); // O(|V|)
		
		// Get correct references for origin and destination
		a = vertices.get(a);
		b = vertices.get(b);
		
		a.setCost(0);

		// Find lowest unknown vertex
		Vertex current = getLowestUnknownVertex(vertices);
		Vertex destination;
		
		// While there are unknown nodes in the map
		while (current != null) { // O(|V|^2)
			current.setKnown(true);
			
			// Go through each adjacent vertex
			for (Vertex adjacent : adjacentVertices(current)) { // O(|E|)
				destination = vertices.get(adjacent);
				int c1 = current.getCost() + edgeCost(current, adjacent);
				int c2 = destination.getCost();
				
				// If the new path calculated is lower cost, swap it!
				if (c1 < c2) {
					destination.setCost(c1);
					destination.setPath(current);
				}
			}
			
			current = getLowestUnknownVertex(vertices);
		}
		
		List<Vertex> path = buildPath(b);
		
		// If the first in the path is the same as the initial vertex
		if (path.get(0).equals(a))
			return new Path(path, b.getCost());

		return null;
	}

	/**
	 * Builds a map that maps vertices to vertices to ensure references
	 * work correctly for the shortest path method. Initializes each vertex's
	 * known field to false, path to null, and cost to infinity.
	 * @return the map of initialized vertices.
	 */
	private Map<Vertex, Vertex> buildInitMap() {
		Map<Vertex, Vertex> vertices = new HashMap<Vertex, Vertex>();

		for (Vertex v : vertices()) {
			v.setCost(Integer.MAX_VALUE);
			v.setPath(null);
			v.setKnown(false);
			vertices.put(v, v);
		}
		
		return vertices;
	}
	
	/**
	 * Finds the lowest unknown vertex in the given map of vertices.
	 * @param vertices the map of vertices to search
	 * @return the lowest unknown vertex in the map
	 */
	private Vertex getLowestUnknownVertex(Map<Vertex, Vertex> vertices) {
		Vertex low = null;
		
		for (Vertex v : vertices.keySet()) {
			if ((low == null || v.getCost() < low.getCost()) && !v.isKnown())
				low = v;
		}
		
		return low;
	}
	
	/**
	 * Builds the path to the given destination vertex, starting with
	 * the origin vertex.
	 * @param destination the destination to start building the path from
	 * @return the path to the destination vertex
	 */
	private List<Vertex> buildPath(Vertex destination) {
		List<Vertex> path = new LinkedList<Vertex>();
		Vertex current = destination;
		while (current != null) {
			path.add(0, current);
			current = current.getPath();
		}
		
		return path;
	}

}
