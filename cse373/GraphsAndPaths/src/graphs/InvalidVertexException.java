package graphs;

/**
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 * 

 * Represents an exception for invalid vertex, specifically for edges
 * with vertices that do not exist in a specific graph. 
 */

public class InvalidVertexException extends IllegalStateException {
	 // assign a version number for serializable class
	 private static final long serialVersionUID = 1L;
	
	 /**
	  * Empty constructor
	  */
	 public InvalidVertexException() {
	 }
	
	 /**
	  * Constructor that takes message as parameter
	  * 
	  * @param message
	  *            the error message
	  */
	 public InvalidVertexException(String message) {
	     super(message);
	 }
}
