package graphs;

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
