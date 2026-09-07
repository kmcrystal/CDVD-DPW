INLINE static void
dtoieee(double *dp)
{
	double_t num;
	double x;
	int exp;
	num.d = *dp;
	if (!num.d) {			
		num.l[0] = num.l[1] = 0;
		return;
	}
	if (num.d < 0) {		
		num.d = -num.d;
		num.ieee.sign = 1;
	} else {
		num.ieee.sign = 0;
	}
	
	x = frexp(num.d, &exp);
	





	if (exp < -1022) {		
		x = ldexp(x, -1023-exp);
		exp = 0;
	} else if (exp > 1023) {	
		x = 0;
		exp = 2047;
	} else {			
		x *= 2;
		x -= 1;
		exp += 1022; 
	}
	num.ieee.exp = exp;
	x *= (double) (1<<20);
	num.ieee.mant = (long) x;
	x -= (double) num.ieee.mant;
	num.ieee.mant2 = (long) (x*((double) (1<<16)*(double) (1<<16)));
	if (!(num.ieee.mant || num.ieee.exp || num.ieee.mant2)) {
		
		num.ieee.sign = 0;
	}
	((double_t*)dp)->ieee = num.ieee;
}
