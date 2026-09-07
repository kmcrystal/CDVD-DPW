static void
compresssep(unsigned char* out,
	    unsigned char* r, unsigned char* g, unsigned char* b, uint32 n)
{
	register uint32 red = RED, green = GREEN, blue = BLUE;
	while (n-- > 0)
		*out++ = (unsigned char)
			((red*(*r++) + green*(*g++) + blue*(*b++)) >> 8);
}
