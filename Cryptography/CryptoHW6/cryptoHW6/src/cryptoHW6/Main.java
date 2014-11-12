package cryptoHW6;

public class Main {

	public static void main(String[] args) {
		
		/*
		 * Output
		 * 
		 * 	Problem 1 (a)

				f7918 mod 7919 is 0
				f999984 mod 999983 is 0
				f999999938 mod 999999937 is 0

			Problem 1 (b)

				Smallest k is 11

			Problem 2

				Smallest k is 9
		 */
		
		System.out.println("Problem 1 (a)\n");
		
		long[] a = {1,1,1,0};
		
		long c = 7919;
		long eps = EpsFunc.EpsFunc(c);
		long b = c - eps;
		long[] ans;
		
		ans = RF.rf(a, b, c);
				
		System.out.println(	"f"+ b + " mod "+ c + " is " +ans[1]+"\n");
		
		c = 1000000-17;
		eps = EpsFunc.EpsFunc(c);
		b = c-eps;
		ans = RF.rf(a,b,c);
		
		System.out.println(	"f"+ b + " mod "+ c + " is " +ans[1]+"\n");
		
		c = 1000000000-63;
		eps = EpsFunc.EpsFunc(c);
		b = c-eps;
		ans = RF.rf(a,b,c);
		
		System.out.println(	"f"+ b + " mod "+ c + " is " +ans[1]+"\n");
		
		System.out.println("Problem 1 (b)\n");
		
		long k = 0;
		long alpha;
		long n = 100000000;
		long p;
		
		while (k < n) {
			p = n - k;
			alpha = Jacobi.Jacobi(5, p);
			if (RF.rf(a, p-alpha, p)[1] == 0) break;
			k++;
		}
		
		System.out.println("Smallest k is "+ k + "\n");
		
		
		System.out.println("Problem 2\n");
		
		long P = 7;
		long Q = 3; 
		long delta = P*P-4*Q;
		long[] B = {P, -Q, 1, 0};
		k = 0;
		n = 10000000;
		long sigma;
		
		while (k < n) {
			p = n - k;
			
			if (p%2 == 0){
				k++;
			}
			
			else if (GCD.gcd(p, Q*delta)!=1) {
				k++;
			}
			
			else {
				sigma = Jacobi.Jacobi(delta, p);
				if (RF.rf(B, p-sigma, p)[1] == 0) break;
				k++;
			}
		}

		System.out.println("Smallest k is "+ k + "\n");
		
		/*
		This is a failed attempt at using the BigInteger library
		BigInteger[] a = {BigInteger.ONE, BigInteger.ONE, BigInteger.ONE, BigInteger.ONE, BigInteger.ZERO};
		
		
		BigInteger c = BigInteger.valueOf(7919);;
		BigInteger eps = EpsFuncBig.EpsFunc(c);
		BigInteger b = c.subtract(eps);
		
		BigInteger[] ans = new BigInteger[4];
		
		ans = RFBig.rf(a, b, c);
		
		System.out.println(	"f"+ b + " mod "+ c + " is " +ans[1]+"\n");
		
		c = BigInteger.valueOf(1000000-17);
		eps = EpsFuncBig.EpsFunc(c);
		b = c.subtract(eps);
		
		ans = RFBig.rf(a, b, c);
		
		System.out.println(	"f"+ b + " mod "+ c + " is " +ans[1]+"\n");
		
		c = BigInteger.valueOf(1000000000-63);
		eps = EpsFuncBig.EpsFunc(c);
		b = c.subtract(eps);
		
		ans = RFBig.rf(a, b, c);
		
		System.out.println(	"f"+ b + " mod "+ c + " is " +ans[1]+"\n");
		*/
	}

}
