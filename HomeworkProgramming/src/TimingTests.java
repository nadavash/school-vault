
public class TimingTests {
	public static final int NUM_TESTS = 5;
	public static final int[] TEST = new int[] {
		10000,
		20000,
		30000,
		40000,
		50000,
	};
	
	public static void main(String[] args) {
		for (int i = 0; i < NUM_TESTS; ++i) {
			long before = System.nanoTime();
			@SuppressWarnings("unused")
			int sumResult = testOne(TEST[i]);
			long delta = System.nanoTime() - before;
			System.out.print(delta + "\t");
		}
		System.out.println();

	}
	
	public static int testOne(int n) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			sum++;
		}
		return sum;
	}
	
	public static int testTwo(int n) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				sum++;
			}
		}
		return sum;
	}
	
	public static int testThree(int n) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < i; j++) {
				sum++;
			}
		}
		return sum;
	}
	
	public static int testFour(int n) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n * n; ++j) {
				sum++;
			}
		}
		return sum;
	}
	
	public static int testFive(int n) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < i; j++) {
				sum++;
			}
			for (int k = 0; k < 8000; k++) {
				sum++;
			}
		}
		return sum;
	}
	
	public static int testSix(int n) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < i*i; j++) {
				if (j % i == 0) {
					for (int k = 0; k < j; k++) {
						sum++;
					}
				}
			}
		}
		return sum;
	}
	
	public static int testSeven(int n) {
		int sum = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < i * i; j++) {
				sum++;
			}
		}
		return sum;
	}
}
