package cryptoHW6;

public class GCD {
	public static long gcd(long a, long b) {
		
		if (a < 0) a = -a;
		if (b < 0) b = -b;

		long temp;

		while (b != 0) {
			temp = a%b;
			a = b;
			b = temp;
		}

		return a;
	}
}
