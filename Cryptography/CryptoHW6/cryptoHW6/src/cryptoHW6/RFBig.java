package cryptoHW6;

import java.math.BigInteger;

public class RFBig {
//This doesn't work for some reason, but we didn't need the big integer library.
	// The recursive exponential function
		public static BigInteger[] rf(BigInteger[] a, BigInteger b, BigInteger c) {
		
		if(b.compareTo(BigInteger.valueOf(0))==0) {
			BigInteger[] id = new BigInteger[4];
			id[0] = BigInteger.valueOf(1);
			id[1] = BigInteger.valueOf(0);
			id[2] = BigInteger.valueOf(0);
			id[3] = BigInteger.valueOf(1);
			
			return id;
		}

		if(b.mod(BigInteger.valueOf(2)).compareTo(BigInteger.valueOf(1)) == 0) {
			BigInteger[] inta = new BigInteger[4];
			BigInteger[] f = new BigInteger[4];
			
			inta = rf(a,b.subtract(BigInteger.valueOf(1)),c);
			
			f[0] = a[0].multiply(inta[0]).add(a[1].multiply(inta[2]));
			f[1] = a[0].multiply(inta[1]).add(a[1].multiply(inta[3]));
			f[2] = a[2].multiply(inta[0]).add(a[3].multiply(inta[2]));
			f[3] = a[2].multiply(inta[1]).add(a[3].multiply(inta[3]));
			
			inta[0] = f[0].mod(c);
			inta[1] = f[1].mod(c);
			inta[2] = f[2].mod(c);
			inta[3] = f[3].mod(c);
			
			return inta;
		}

		BigInteger[] d = new BigInteger[4];
		d = rf(a, b.divide(BigInteger.valueOf(2)), c);
		BigInteger[] e = new BigInteger[4];
		
		e[0] = d[0].multiply(d[0]).add(d[1].multiply(d[2]));
		e[1] = d[0].multiply(d[1]).add(d[1].multiply(d[3]));
		e[2] = d[2].multiply(d[0]).add(d[3].multiply(d[2]));
		e[3] = d[2].multiply(d[1]).add(d[3].multiply(d[3]));
		
		d[0] = e[0].mod(c);
		d[1] = e[1].mod(c);
		d[2] = e[2].mod(c);
		d[3] = e[3].mod(c);
		
		return d;
		}
	
}
