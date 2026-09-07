void
TIFFSwabArrayOfLong(register uint32* lp, tmsize_t n)
{
	register unsigned char *cp;
	register unsigned char t;
	assert(sizeof(uint32)==4);
	
	while (n-- > 0) {
		cp = (unsigned char *)lp;
		t = cp[3]; cp[3] = cp[0]; cp[0] = t;
		t = cp[2]; cp[2] = cp[1]; cp[1] = t;
		lp++;
	}
}
