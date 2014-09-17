#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace std;
	
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

	//Computing 3 <= n <= 7919
	long compute2A(long a) {
		
		for (long i = 3; i < 7920; i++) {

			itf(a, i-1, i);
		}

		return 0;

	}
	//Modified Program
	long compute2B(long a) {

		long count = 0; 

		for (long i = 3; i < 7920; i++) {

			if (itf(a, i-1, i) == 1) count++;
		}

		return count;

	}

	int main() {
		long a = 2;
		long b = 252;
		long c = 1009;

		cout << "The value of a is: " << a << endl;
		cout << "The value of b is: " << b << endl;
		cout << "The value of c is: " << c << endl;

		cout << "re a^b mod c is: " << rf(a,b,c) << endl;
		cout << "it a^b mod c is: " << itf(a,b,c) << endl;

		a = 2;
		b = 504;
		c = 1009;

		cout << "The value of a is: " << a << endl;
		cout << "The value of b is: " << b << endl;
		cout << "The value of c is: " << c << endl;

		cout << "a^b mod c is: " << rf(a,b,c) << endl;
		cout << "it a^b mod c is: " << itf(a,b,c) << endl;

		a = 2;
		b = 1008;
		c = 1009;

		cout << "The value of a is: " << a << endl;
		cout << "The value of b is: " << b << endl;
		cout << "The value of c is: " << c << endl;

		cout << "a^b mod c is: " << rf(a,b,c) << endl;
		cout << "it a^b mod c is: " << itf(a,b,c) << endl;
		
		a = 2;
		cout 	<< "Number of times N = 1 is: " 
			<< compute2B(a) << endl;

		cout << "7919/log_e(7919) is: " << 7919 / log(7919) << 			endl;

		if (compute2B(a) > 7919/log(7919))
			cout << "N+1 > 7919/ln(7919)" << endl;

		return 0;
	}
