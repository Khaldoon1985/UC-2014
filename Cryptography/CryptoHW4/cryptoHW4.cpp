#include <stdio.h>
#include <iostream>
#include <math.h>
#include <ctype.h>
#include <gmp.h>
#include "gmpxx.h"

using namespace std;

	//Euclid's GCD
	long gcd(long a, long b) {

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

	//Mod n Inverse
	long inverse(long a, long b) {
		// if gcd(a,b) = 1, it returns a^-1, 0 < a^-1 < b, such that a*a^-1 ≡ 1 (mod b)

		if (b < 0) b = -b;
		a = a%b;

		if (a < 0) a = a+b;
		
		long u1, u2, u3, v1, v2, v3, t1, t2, t3;

		long q;

		//initialize
		u1 = 1;
		u2 = 0;
		u3 = a%b;
		v1 = 0;
		v2 = 1;
		v3 = b;

		//if u is too big, reduce it mod v
		u3 = u3%v3;

		while (v3 != 0) {
			//cout<<"\n entered while loop with q = "<<q<<"v3 ="<<v3;
			q=u3/v3;
			t1 = u1 - v1*q;
			t2 = u2 - v2*q;
			t3 = u3 - v3*q;
			u1=v1; u2=v2; u3=v3;
			v1=t1;v2=t2;v3=t3;
		}

		u1=u1%b;
		if(u1<0)u1=u1+b;

		return u1;
	}

	//Find factor
	long getFactorSub(long x, long n) {
		return (x*x+1)%n;
	}

	long getFactor(long n) {
		long x_fixed = 2;
		long cycle_size = 2;
		long x = 2;
		long h = 1;
		while (h == 1) {
			long count = 1;
			while (count <= cycle_size && h == 1) {
				x = getFactorSub(x,n);
				count = count + 1;
				h = gcd(x-x_fixed, n);
			}
			if (h != 1) break;
			cycle_size = 2 * cycle_size;
			x_fixed = x;
		}
		return h;
	}

	//Chinese
	long chinese(long a, long b, long m, long n) {
	// Returns x such that 0 < x < mn and x ≡ a (mod m), and x ≡ b (mod n).
	//Must have gcd(m,n) = 1.
	long x,u,v,i,r;

	i = inverse(m,n);

	if(i==0) return 0;
	u=a%m;

	if(u<0)u=u+m;
	v=b%n;

	if(v<0)v=v+n;
	x = u+m*(v-u)*i;
	r=m*n;
	x=x%r;

	if(x<0)x=x+r;

	return x;
	}


	//The following is a deterministic algorithm to determine primality. It is not practical for large numbers n.
	int isPrime(int n) {
		//returns the value 1 if n is a prime, the value 0 otherwise.
		for(int d = 2; d*d <= n; ++d) if(n%d == 0) return 0;
		return 1;
	}

	//The following is an algorithm for pi(x) that uses the function isPrime(n).
	int pi(double x) {
		//cout << "Entered pi(x)" << endl;
		//Returns the value of the number of primes not exceeding x.
		int count = 1; //2 is the only even prime.
		for(int j = 3; j <= x; j = j + 2) {
			if (isPrime(j)) {
				//cout << j << " is prime." << endl;
				++count;
			}
		}
		return count;
	}

	//The following algorithm uses isPrime(n) to determine whether a number is safe prime. The first few safe primes are 5, 7, 11, ...
	int isSafePrime(int n) {
		int flag = isPrime(n);
		if(!flag) return 0; //if n isn't a prime, then it isn't a safe prime.
		flag = isPrime((n-1)/2);
		if(!flag) return 0; //if (n-1)/2 isn't prime, then n is an unsafe prime.
		return 1;
	}

	//The following algorithm counts the number of Pythagorean primes. These are the primes that are congruent to 1 modulo 4. The first few are 5, 13, 17, 29, ...
	int p14(double x) {
		//cout << "Entered p14(x)" << endl;
		int count = 0;
		for(int j = 5; j <= x; j = j + 4)
			if(isPrime(j)) {
				//cout << j << " is 14 Prime." << endl;
				++count;
			}
		return count;
	}

	//The following algorithm counts the number of Pythagorean primes. These are the primes that are congruent to 3 modulo 4. The first few are 3, 7, 11, ...
	int p34(double x) {
		//cout << "Entered p34(x)" << endl;
		int count = 0;
		for(int j = 3; j <= x+1; j = j + 4)
			if(isPrime(j)) {
				//cout << j << " is 34 Prime." << endl;
				++count;
			}
		return count;
	}

	//The following algorithm finds the number of safe primes not exceeding x by using the function isSafePrime(n).
	int piSafe(double x) {
		//cout << "Entered piSafe(x)" << endl;
		//This will count the number of safe primes, beginning with 7.
		int count = 0;
		for(int j = 5; j <= x; j = j + 2)
			if(isSafePrime(j)) ++count;
		return count;
	}

	//HW4Problem1
	int tFinder(int p, int s) {
		//s ≡ t^2 (mod p)
		int flag = 0;
		int x = 1;
		while (flag == 0) {
			int xSquared = x * x;
			if (xSquared%p == s) {
				flag = 1;
			}
			else {
			x = x + 1;
			}
		}
		return x;
	}


	// The recursive exponential function
	long rf(long a, long b, long c) {
	
		if(b==0) {
			//cout << "The rf returned 1." << endl;
			return 1;
		}

		if(b%2==1) {
			//cout << "The rf returned 1." << endl;
			return (a*rf(a,b-1,c) )%c;
		}

		long d = rf(a, b/2, c);
		//cout << "Returning number: " << d << endl;
		return (d*d)%c;
	}

		// The iterative exponential function
	long itf(long a, long b, long c) {

		long e = 1;
		while (b != 0) {

			if (b%2 == 1) {
				e = (e*a)%c;
				b = b-1;			
			}

			a = (a*a)%c;
			b = b/2;
		}

		return e;

	}
	//HWProblem2
	void p1Factor(){

		//mpz_sub(r,k,g);
		//mpz_powm(d,a,b,c);
		//mpz_gcd(d,a,b);
		//mpz_add(d,a,c);
		//if first is bigger, 1
		//if second is bigger, -1
		//if equal, 0
		//int w = mpz_cmpabs(g,r);

		//gmp_printf("Here it is %Zd\n", r);

		mpz_t g,gn,go,r,n,h,k;
		mpz_inits(g,gn,go,r,n,h,k,NULL);

		mpz_set_str(g, "2", 10);
		mpz_set_str(r, "2", 10);
		mpz_set_str(k, "38", 10);
		mpz_set_str(n, "190248273382547686244479775579416295505415044511", 10);
		mpz_set_str(go,"1",10);

		gmp_printf("%Zd has following divisors:\n",n);

		while (mpz_cmpabs(r,k) == -1 || mpz_cmpabs(r,k) == 0) {
			if (mpz_cmpabs(h,go) > 0 && mpz_cmpabs(h,n) < 0) {
				gmp_printf("%Zd is a divisor.\n",h);
				mpz_cdiv_q(n,n,h);
				gmp_printf("%Zd is a divisor.\n",n);
			}
			mpz_powm(g,g,r,n);
			mpz_sub(gn,g,go);
			mpz_gcd(h,gn,n);
			mpz_add(r,r,go);
		}

	}

	void primRoot() {	

		mpz_t base,pSafe,k,q,n,a,aSquared,aQ,modResult,modResult2,One,Two;
		mpz_inits(base,pSafe,q,k,n,a,aSquared,aQ,modResult,modResult2,One,Two,NULL);
		mpz_set_str(base, "10", 10);
		mpz_set_str(k, "1879", 10);
		mpz_set_str(n, "0", 10);
		mpz_set_str(a, "2", 10);
		mpz_set_str(One, "1", 10);
		mpz_set_str(Two, "2", 10);

		mpz_pow_ui(pSafe,base,25);
		mpz_add(pSafe,pSafe,k);
		gmp_printf("\nIt is given that %Zd is a safe prime.\n",pSafe);

		//Since we have a safe prime, we have to check that neither a^2 ≡ 1 (mod p) nor a^q ≡ 1 (mod p).


		int counter = 0;

		while (counter < 43) {
			mpz_mul(aSquared,a,a);
			mpz_mod(modResult,aSquared,pSafe);
			mpz_sub(q,pSafe,One);
			mpz_cdiv_q(q,q,Two);
			mpz_powm(modResult2,a,q,pSafe);
			if (mpz_cmpabs(modResult,One) != 0 && mpz_cmpabs(modResult2,One) != 0) {
				gmp_printf("%Zd is a primitive modulo.\n", a);
			}
			mpz_add(a,a,One);
			counter++;
		}

	}

	int main() {

		//HW4 Problem 1
		//Prime p = 4k+3, quadratics residue s modulo p. Find t such that s ≡ t^2 (mod p). 
		//p = 107.  s = 99

		cout << "\nHomework4 Problem 1\nm" << endl;
		int p = 107;
		int s = 99;
		cout << "p and s are: " << p << "," << s << endl;
		cout << "t is " << tFinder(p,s) << endl;

		//p = 1039,  s = 262
		p = 1039;
		s = 262;
		cout << "p and s are: " << p << "," << s << endl;
		cout << "t is " << tFinder(p,s) << endl;

		//p = 2707, s = 1712
		p = 2707;
		s = 1712;
		cout << "p and s are: " << p << "," << s << endl;
		cout << "t is " << tFinder(p,s) << endl;

		//HW4 Problem 2
		//Factor the number n using the "p-1 method."
		//n = 190 248 273 382 547 686 244 479 775 579 416 295 505 415 044 511.
		//[Hint:  n=pq, where p and q are prime and p-1 is 37-smooth.]
		
		cout << "\nHomework4 Problem 2\n" << endl;		
		p1Factor();
		
		//HW4 Problem 3
		//Find the smallest primitive root modulo  Psafe = 10^25 + k, where k = 1879.
		//If p is a prime and ord(a,p) = p-1, then we say that a is a primitive root modulo p.
		
		cout << "\nHomework4 Problem 3\n" << endl;
		primRoot();
		cout << "23 is the smallest primitive root modulo." << endl;

	}
