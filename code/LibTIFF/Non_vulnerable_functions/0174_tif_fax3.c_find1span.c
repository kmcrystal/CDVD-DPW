inline static int32
find1span(unsigned char* bp, int32 bs, int32 be)
{
	int32 bits = be - bs;
	int32 n, span;
	bp += bs>>3;
	


	if (bits > 0 && (n = (bs & 7))) {
		span = oneruns[(*bp << n) & 0xff];
		if (span > 8-n)		
			span = 8-n;
		if (span > bits)	
			span = bits;
		if (n+span < 8)		
			return (span);
		bits -= span;
		bp++;
	} else
		span = 0;
	if (bits >= (int32)(2 * 8 * sizeof(long))) {
		long* lp;
		


		while (!isAligned(bp, long)) {
			if (*bp != 0xff)
				return (span + oneruns[*bp]);
			span += 8, bits -= 8;
			bp++;
		}
		lp = (long*) bp;
		while ((bits >= (int32)(8 * sizeof(long))) && (~0 == *lp)) {
			span += 8*sizeof (long), bits -= 8*sizeof (long);
			lp++;
		}
		bp = (unsigned char*) lp;
	}
	


	while (bits >= 8) {
		if (*bp != 0xff)	
			return (span + oneruns[*bp]);
		span += 8, bits -= 8;
		bp++;
	}
	


	if (bits > 0) {
		n = oneruns[*bp];
		span += (n > bits ? bits : n);
	}
	return (span);
}
