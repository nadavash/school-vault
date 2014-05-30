package graphs;

/**
 * @author Quynh Huynh, Nadav Ashkenazi
 * @UWNetID quhuynh, nadava
 * @studentID 1120428, 1230523
 * @email quhuynh@uw.edu, nadava@uw.edu
 * 

 * Represents an exception for negative weight edges. Edges in the Graph
 * class can only have a cost of zero or more.
 */
public class NegativeWeightException extends IllegalStateException {
	// assign a version number for serializable class
    private static final long serialVersionUID = 1L;

    /**
     * Empty constructor
     */
    public NegativeWeightException() {
    }

    /**
     * Constructor that takes message as parameter
     * 
     * @param message
     *            the error message
     */
    public NegativeWeightException(String message) {
        super(message);
    }

}
