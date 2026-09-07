void
TIFFSwabShort(uint16* wp)
{
	register unsigned char* cp = (unsigned char*) wp;
	unsigned char t;
	assert(sizeof(uint16)==2);
	t = cp[1]; cp[1] = cp[0]; cp[0] = t;
}
