package writeupExperiment;

import providedCode.*;

public class BadStringHasher implements Hasher {

	/**
	 * TODO: Modify this comment
	 * Generates a hash code for the provided string.
	 * @Returns a hash code for the provided string
	 */
	@Override
	public int hash(String str) {
		int hash = 0;

		for (int i = str.length() - 1; i >= 0; --i) {
			hash += str.charAt(i);
		}
		
		return hash >= 0 ? hash : -hash;
	}

}
