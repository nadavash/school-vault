/**
 * 
 */
package graphs;

/**
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 * 
<<<<<<< HEAD
<<<<<<< HEAD
 * Thrown when there are duplicate edges
=======
 * Represents an exception for invalid duplicate edges in a Graph object. Invalid
 * duplicate edges are edges the have identical source and destination but a different
 * cost.
>>>>>>> 20ed9867acc0cbe28375a2df62b0149e6358e1b9
=======
 * Represents an exception for invalid duplicate edges in a Graph object. Invalid
 * duplicate edges are edges the have identical source and destination but a different
 * cost.
>>>>>>> 20ed9867acc0cbe28375a2df62b0149e6358e1b9
 */

<<<<<<< HEAD
<<<<<<< HEAD
public class InvalidDuplicateEdgeException extends IllegalStateException {
	// assign a version number for serializable class
=======
>>>>>>> 20ed9867acc0cbe28375a2df62b0149e6358e1b9
=======
>>>>>>> 20ed9867acc0cbe28375a2df62b0149e6358e1b9
	private static final long serialVersionUID = 1L;

	/**
	  * Empty constructor
	  */
	 public InvalidDuplicateEdgeException() {
	 }
	
	 /**
	  * Constructor that takes message as parameter
	  * 
	  * @param message
	  *            the error message
	  */
	 public InvalidDuplicateEdgeException(String message) {
	     super(message);
	 }
}
