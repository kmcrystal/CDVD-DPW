INLINE static void
ieeetod(double *dp)
{
	double_t source;
	long sign,exp,mant;
	double dmant;
	source.ieee = ((double_t*)dp)->ieee;
	sign = source.ieee.sign;
	exp = source.ieee.exp;
	mant = source.ieee.mant;
	if (exp == 2047) {
		if (mant)			
			*dp = HUGE_VAL;
		else				
			*dp = (sign ? -HUGE_VAL : HUGE_VAL);
		return;
	}
	if (!exp) {
		if (!(mant || source.ieee.mant2)) {	
			*dp=0;
			return;
		} else {			
			
			exp= -1022;
		}
	} else {
		mant |= 1<<20;
		exp -= 1023;
	}
	dmant = (((double) mant) +
		((double) source.ieee.mant2) / (((double) (1<<16)) *
		((double) (1<<16)))) / (double) (1<<20);
	dmant = ldexp(dmant, exp);
	if (sign)
		dmant= -dmant;
	*dp = dmant;
}
