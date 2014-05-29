/**
 * 
 */
package graphs;

/**
 * @author nadavash
 * TODO
 */
public class InvalidDuplicateEdgeException extends IllegalStateException {

	/**
	 * TODO
	 */
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
