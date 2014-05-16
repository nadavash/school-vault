package shake_n_bacon;

import providedCode.Hasher;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu
 * This StringHasher class represents a hasher that hashes a string
 * similarly to the Java method.
 */
public class StringHasher implements Hasher {
	public static final int HASH_FACTOR = 37;
	
	/**
	 * Generates a hash code for the provided string. Hash is generated
	 * by adding each letter's ascii value multiplied by a factor of 37
	 * to the power of the letter's position.
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
