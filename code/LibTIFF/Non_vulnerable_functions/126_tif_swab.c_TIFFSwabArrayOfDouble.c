void
TIFFSwabArrayOfDouble(double* dp, tmsize_t n)
{
	register unsigned char *cp;
	register unsigned char t;
	assert(sizeof(double)==8);
	
	while (n-- > 0) {
		cp = (unsigned char *)dp;
		t = cp[7]; cp[7] = cp[0]; cp[0] = t;
		t = cp[6]; cp[6] = cp[1]; cp[1] = t;
		t = cp[5]; cp[5] = cp[2]; cp[2] = t;
		t = cp[4]; cp[4] = cp[3]; cp[3] = t;
		dp++;
	}
}
