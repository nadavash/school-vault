package shake_n_bacon;

import providedCode.*;

/**
 * @author Nadav Ashkenazi
 * @UWNetID nadava
 * @studentID 1230523
 * @email nadava@uw.edu
 * This class represents a Comparator that compares two strings.
 */
public class StringComparator implements Comparator<String> {

	/**
	 * Compares two strings against each other. Returns a value
	 * bigger than 0 if s1 > s2, 0 if s1 = s2, and < 0 if s1 < s2.
	 */
	@Override
	public int compare(String s1, String s2) {
		char c1, c2;
		int minLength = Math.min(s1.length(), s2.length());
		
		for (int i = 0; i < minLength; ++i) {
			c1 = s1.charAt(i);
			c2 = s2.charAt(i);
			
			if (c1 != c2) {
				return c1 - c2;
			}
		}
		
		return s1.length() - s2.length();
	}
}
