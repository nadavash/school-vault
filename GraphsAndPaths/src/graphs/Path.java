package graphs;

import java.util.List;

/** 
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 *
 * Stores a given path and its cost.
 */
public class Path {
	public final List<Vertex> vertices; // stores the vertices of this path
	public final int cost; // stores the total cost of this path

	public Path(List<Vertex> vertices, int cost) {
		this.vertices = vertices;
		this.cost = cost;
	}
}
