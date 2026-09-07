static void
compresspalette(unsigned char* out, unsigned char* data, uint32 n, uint16* rmap, uint16* gmap, uint16* bmap)
{
	register int v, red = RED, green = GREEN, blue = BLUE;
	while (n-- > 0) {
		unsigned int ix = *data++;
		v = red*rmap[ix];
		v += green*gmap[ix];
		v += blue*bmap[ix];
		*out++ = v>>8;
	}
}
