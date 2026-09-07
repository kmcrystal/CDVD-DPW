void
TIFFSwabArrayOfShort(register uint16* wp, tmsize_t n)
{
	register unsigned char* cp;
	register unsigned char t;
	assert(sizeof(uint16)==2);
	
	while (n-- > 0) {
		cp = (unsigned char*) wp;
		t = cp[1]; cp[1] = cp[0]; cp[0] = t;
		wp++;
	}
}
