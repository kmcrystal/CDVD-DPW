void
TIFFSwabFloat(float* fp)
{
	register unsigned char* cp = (unsigned char*) fp;
	unsigned char t;
	assert(sizeof(float)==4);
	t = cp[3]; cp[3] = cp[0]; cp[0] = t;
	t = cp[2]; cp[2] = cp[1]; cp[1] = t;
}
