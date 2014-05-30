package graphs;

/**
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 * 
<<<<<<< HEAD
 * Thrown when an edge contains an invalid vertex.
 */

=======
 * Represents an exception for invalid vertex, specifically for edges
 * with vertices that do not exist in a specific graph. 
 */
>>>>>>> 20ed9867acc0cbe28375a2df62b0149e6358e1b9
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
