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
 * Represents an exception for invalid duplicate edges in a Graph object. Invalid
 * duplicate edges are edges the have identical source and destination but a different
 * cost.
 */
public class InvalidDuplicateEdgeException extends IllegalStateException {

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
