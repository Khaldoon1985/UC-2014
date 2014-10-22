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
		//Homework 5
		//Crypto #8 Inverse multiplication
		//Find  A, B, and M given the following five consecutive terms in the sequence 21.1:
		//x[0]	=	56687054115473550533
		//x[1]	=	71501923691929981066
		//x[2]	=	 1162551557687152936
		//x[3]	=	88117163952857350660
		//x[4]	=	16754986973331962115		
		//To help you test your answer for correctness, here is x[10]:
		//x[10]	=  	66775494619620378439.
		//You must hand in x[5] as well as A,B, and M.
		//(21.1) X_n+1 ≡ AX_n + B (Mod M)
		//X1 ≡ AX0 + B (Mod M)
		//X2 ≡ AX1 + B (Mod M)
		//X3 ≡ AX2 + B (Mod M)
		//X4 ≡ AX3 + B (Mod M)
		//X1-X2 ≡ A(X0-X1) (Mod M)
		//X3-X4 ≡ A(X2-X3) (Mod M)
		//mpz_mod(AX0+B,X1,M);

		mpz_t x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x01,x12,x23,x34,D0,D1,a,b,c,d,m,m2,m3,m6,m29,m174,m577,m100398,m210323,intCalc,intCalc2;
		mpz_inits(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x01,x12,x23,x34,D0,D1,a,b,c,d,m,m2,m3,m6,m29,m174,m577,m100398,m210323,intCalc,intCalc2,NULL);

		mpz_set_str(x0, "56687054115473550533", 10);
		mpz_set_str(x1, "71501923691929981066", 10);
		mpz_set_str(x2, "1162551557687152936", 10);
		mpz_set_str(x3, "88117163952857350660", 10);
		mpz_set_str(x4, "16754986973331962115", 10);
		mpz_set_str(x5, "2", 10);
		mpz_set_str(x6, "2", 10);
		mpz_set_str(x7, "2", 10);
		mpz_set_str(x8, "2", 10);
		mpz_set_str(x9, "2", 10);
		mpz_set_str(x10, "66775494619620378439", 10);

		mpz_sub(x01,x1,x0);
		mpz_sub(x12,x2,x1);
		mpz_sub(x23,x2,x3);
		mpz_sub(x34,x3,x4);
		mpz_gcd(a,x01,x12);
		
		//[X0 X1 1; X1 X2 1; X2 X3 1] D0
		//[X1 X2 1; X2 X3 1; X3 X4 1] D1
		//M = gcd(D0, D1);
		//It is possible that h = aM;
		//[a b c; d e f; g h i]
		//Det = a(ei-fh) - b(di-fg) + c(dh-eg)
		//a = X0 = X1;
		//b = X1 = X2;
		//c =  1 =  1;
		//d = X1 = X2;
		//e = X2 = X3;
		//f =  1 =  1;
		//g = X2 = X3;
		//h = X3 = X4;
		//i =  1 =  1;

		//D0 = X0*(X2*1-1*X3) - X1*(X1*1-1*X2) + 1*(X1*X3-X2*X2);
		
		mpz_sub(intCalc,x2,x3);
		mpz_mul(D0,x0,intCalc);

		mpz_sub(intCalc,x1,x2);
		mpz_mul(intCalc,x1,intCalc);
		mpz_sub(D0,D0,intCalc);

		mpz_mul(intCalc,x1,x3);
		mpz_mul(intCalc2,x2,x2);
		mpz_sub(intCalc,intCalc,intCalc2);
		mpz_add(D0,D0,intCalc);

		//D1 = X1*(X3*1-1*X4) - X2*(X2*1-1*X3) + 1*(X2*X4-X3*X3);

		mpz_sub(intCalc,x3,x4);
		mpz_mul(D1,x1,intCalc);

		mpz_sub(intCalc,x2,x3);
		mpz_mul(intCalc,x2,intCalc);
		mpz_sub(D1,D1,intCalc);

		mpz_mul(intCalc,x2,x4);
		mpz_mul(intCalc2,x3,x3);
		mpz_sub(intCalc,intCalc,intCalc2);
		mpz_add(D1,D1,intCalc);
		
		mpz_gcd(m,D0,D1);

		mpz_set_str(intCalc, "2", 10);
		mpz_div(m2,m,intCalc);

		mpz_set_str(intCalc, "3", 10);
		mpz_div(m3,m,intCalc);

		mpz_set_str(intCalc, "6", 10);
		mpz_div(m6,m,intCalc);

		mpz_set_str(intCalc, "29", 10);
		mpz_div(m29,m,intCalc);

		mpz_set_str(intCalc, "174", 10);
		mpz_div(m174,m,intCalc);

		mpz_set_str(intCalc, "577", 10);
		mpz_div(m577,m,intCalc);

		mpz_set_str(intCalc, "100398", 10);
		mpz_div(m100398,m,intCalc);

		mpz_set_str(intCalc, "210323", 10);
		mpz_div(m210323,m,intCalc);

		gmp_printf("D0 is %Zd\n",D0);
		gmp_printf("D1 is %Zd\n",D1);
		gmp_printf("M is %Zd\nM has factors 2, 3, and 29.\n",m);
		gmp_printf("M/2 is %Zd.\nM/3 is %Zd.\nM/6 is %Zd.\nM/29 is %Zd.\nM/174 is %Zd.\nM/577 is %Zd.\nM/100398 is %Zd.\n",m2,m3,m6,m29,m174,m577,m100398);
		

		
		int flag = 0;
		
		mpz_set_str(a, "", 10);
		char aAnswer[1000];

		//A * X01 = X12 Mod M
		while (flag == 0) {
			gmp_printf("Trying a = %Zd\n",a);
			gmp_printf("X2 - X1 mod M2 = %Zd\n",intCalc);
			mpz_mul(b,a,x01);
			mpz_mod(b,b,m2);
			if(mpz_cmpabs(b,intCalc) == 0) {
				gmp_printf("Found a %Zd\n",a);
				mpz_get_str(aAnswer,10,a);
				break;
			}
			mpz_add(a,a,m2);
			break;
		}
		mpz_set_str(a,aAnswer,10);
		//A * X0 + B = X1 Mod M

		mpz_set_str(b,"0",10);

		while (flag == 0) {
			break;
			gmp_printf("Trying b = %Zd\n",b);
			
			gmp_printf("a is %Zd\n",a);
			mpz_mul(c,a,x0);
			mpz_add(d,b,c);
			gmp_printf("A * X0 + B = %Zd\n",d);
			mpz_mod(d,d,m2);
			gmp_printf("The Mod is %Zd\n",d);
			gmp_printf("X1 is %Zd\n",x1);
			if(mpz_cmpabs(d,x1) == 0) {
				gmp_printf("Found b %Zd\n",b);
				break;
			}
			mpz_add(b,b,m2);	
			gmp_printf("m2 is %Zd\n",m2);
		}
		
		mpz_set_str(a, "12630192673789351314", 10);
		mpz_set_str(b,"35234390061212433526",10);

		mpz_mul(intCalc,a,x4);
		mpz_add(x5,intCalc,b);
		mpz_mod(x5,x5,m2);
		gmp_printf("%Zd\n",x5);

		mpz_mul(intCalc,a,x5);
		mpz_add(x6,intCalc,b);
		mpz_mod(x6,x6,m2);
		gmp_printf("%Zd\n",x6);

		mpz_mul(intCalc,a,x6);
		mpz_add(x7,intCalc,b);
		mpz_mod(x7,x7,m2);
		gmp_printf("%Zd\n",x7);

		mpz_mul(intCalc,a,x7);
		mpz_add(x8,intCalc,b);
		mpz_mod(x8,x8,m2);
		gmp_printf("%Zd\n",x8);

		mpz_mul(intCalc,a,x8);
		mpz_add(x9,intCalc,b);
		mpz_mod(x9,x9,m2);
		gmp_printf("%Zd\n",x9);

		mpz_mul(intCalc,a,x9);
		mpz_add(x10,intCalc,b);
		mpz_mod(x10,x10,m2);
		gmp_printf("%Zd\n",x10);
		/*
		mpz_set_str(intCalc2,"0",10);
		mpz_sub(x01, intCalc2, x01);
		gmp_printf("x01 is %Zd.\n",x01);

		mpz_mod(intCalc,x1,m2);
		mpz_mod(intCalc2,x2,m2);
		gmp_printf("x1 is %Zd.\n",x1);
		gmp_printf("x2 is %Zd.\n",x2);
		gmp_printf("x1 mod m is %Zd.\n",intCalc);
		gmp_printf("x2 mod m is %Zd.\n",intCalc2);
		
		mpz_sub(intCalc,intCalc,intCalc2);
		mpz_set_str(intCalc2,"0",10);
		mpz_sub(intCalc2, intCalc2, x01);

		mpz_mod(intCalc2,intCalc,intCalc2);
		gmp_printf("A is maybe %Zd\n",intCalc2);
		*/
		
		//Inverse Function
		//Assign the caculated inverse mod to the intCalc
		/*char arbB[1000];
		mpz_get_str(arbB, 10, x01);
		char arbA[1000];
		mpz_get_str(arbA, 10, x12);
		char invModResult[1000];

		mpz_gcd(intCalc,arbA, arbB);
		mpz_set_str(intCalc2, "1", 10);

		if(mpz_cmpabs(intCalc,intCalc2)==0) {
			cout << "gcd(a,b) = 1" << endl;
			invModResult = inverseMod(arbA, M);
		}

		mpz_set_str(intCalc, invModResult, 10);
		gmp_printf("a mod inverse is %Zd\n",intCalc);
		mpz_mul(intCalc,x01,intCalc);
		mpz_set_str(intCalc2, "0", 10);
		if(mpz_cmpabs(intCalc,intCalc2) == 0) {
			gmp_printf("It divides!\n");
		}
		else {
			gmp_printf("x01 and x12 don't have 1 gcd.\n");
		}*/
		/*mpz_set_str(m2,"210323",10);
		//mpz_div(m2,m577,m2);
		gmp_printf("x12 is %Zd\n",x12);
		mpz_mod(intCalc,x12,m2);
		//mpz_add(intCalc,intCalc,m6);
		gmp_printf("x12 mod m100398 is %Zd\n",intCalc);
		mpz_set_str(intCalc2, "0", 10);
		mpz_sub(x01, intCalc2, x01);
		mpz_mod(intCalc2, intCalc, x01);
		gmp_printf("Ax01 mod x01 is %Zd\n",intCalc2);
		int count = 0;
		mpz_set_str(intCalc2, "1", 10);
		mpz_set_str(intCalc, "2", 10);
		mpz_set_str(b,"0",10);
		
		while(count == 0) {
			mpz_mod(a,m2,intCalc);
			if (mpz_cmpabs(a,b) == 0) {
				gmp_printf("divisor is %Zd\n",intCalc);
				count = 1;
			}
			else {
				mpz_add(intCalc, intCalc, intCalc2);
			}
		}*/
		
	}
