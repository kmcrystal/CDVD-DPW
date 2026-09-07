void
TIFFSwabLong8(uint64* lp)
{
	register unsigned char* cp = (unsigned char*) lp;
	unsigned char t;
	assert(sizeof(uint64)==8);
	t = cp[7]; cp[7] = cp[0]; cp[0] = t;
	t = cp[6]; cp[6] = cp[1]; cp[1] = t;
	t = cp[5]; cp[5] = cp[2]; cp[2] = t;
	t = cp[4]; cp[4] = cp[3]; cp[3] = t;
}
