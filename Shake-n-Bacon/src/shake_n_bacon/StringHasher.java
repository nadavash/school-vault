package shake_n_bacon;

import providedCode.Hasher;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu
 */
public class StringHasher implements Hasher {
	public static final int HASH_FACTOR = 37;
	
	/**
	 * TODO: Modify this comment
	 * Generates a hash code for the provided string.
	 * @Returns a hash code for the provided string
	 */
	@Override
	public int hash(String str) {
		int hash = 0;

		int factor = 1;
		for (int i = str.length() - 1; i >= 0; --i) {
			hash += str.charAt(i) * factor;
			factor *= HASH_FACTOR;
		}
		
		return hash >= 0 ? hash : -hash;
	}
}
