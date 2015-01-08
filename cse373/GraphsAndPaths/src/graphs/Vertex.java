package graphs;


/**
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 * 
 * Representation of a graph vertex
 */
public class Vertex {
	private String label; // label attached to this vertex
	private int cost; // cost of this vertex
	private Vertex path; // lowest-cost path to this vertex
	private boolean known; // if the lowest-cost path to this vertex is known

	/**
	 * Construct a new vertex
	 * 
	 * @param label
	 *            the label attached to this vertex
	 */
	public Vertex(String label) {
		if (label == null)
			throw new IllegalArgumentException("null");
		this.label = label;
	}

	/**
	 * Get a vertex label
	 * 
	 * @return the label attached to this vertex
	 */
	public String getLabel() {
		return label;
	}

	/**
	 * A string representation of this object
	 * 
	 * @return the label attached to this vertex
	 */
	public String toString() {
		return label;
	}

	/**
	 * @return The hash code for this vertex
	 */
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((label == null) ? 0 : label.hashCode());
		return result;
	}

	/** 
	 * @param The object you want to compare
	 * @return If the two objects are equal
	 */
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		final Vertex other = (Vertex) obj;
		if (label == null) {
			return other.label == null;
		} else {
			return label.equals(other.label);
		}
	}
	
	/**
	 * Sets the cost of the vertex.
	 * @param cost the cost to set the vertex to
	 */
	public void setCost(int cost) {
		this.cost = cost;
	}
	
	/**
	 * Gets the cost of the vertex.
	 * @return the cost of the vertex
	 */
	public int getCost() {
		return cost;
	}
	
	/**
	 * Sets the previous vertex for this vertex's path.
	 * @param v the previous vertex
	 */
	public void setPath(Vertex v) {
		this.path = v;
	}
	
	/**
	 * Gets the path of the vertex.
	 * @return The lowest-cost vertex that points to this vertex
	 */
	public Vertex getPath() {
		return path;
	}
	
	/**
	 * Checks if the vertex is known.
	 * @return true if the vertex is known, false otherwise
	 */
	public boolean isKnown() {
		return known;
	}
	
	/**
	 * Sets the known value for this vertex.
	 * @param known the value to set the known field to.
	 */
	public void setKnown(boolean known) {
		this.known = known;
	}
}
