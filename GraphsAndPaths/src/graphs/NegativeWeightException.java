package graphs;

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
