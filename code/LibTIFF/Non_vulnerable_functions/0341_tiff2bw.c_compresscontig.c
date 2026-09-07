static void
compresscontig(unsigned char* out, unsigned char* rgb, uint32 n)
{
	register int v, red = RED, green = GREEN, blue = BLUE;
	while (n-- > 0) {
		v = red*(*rgb++);
		v += green*(*rgb++);
		v += blue*(*rgb++);
		*out++ = v>>8;
	}
}
