static int
SeparateCompare(int reversed, int sample, uint32 row,
		unsigned char* cp1, unsigned char* p2)
{
	uint32 npixels = imagewidth;
	int pixel;
	cp1 += sample;
	for (pixel = 0; npixels-- > 0; pixel++, cp1 += samplesperpixel, p2++) {
		if (*cp1 != *p2) {
			printf("Scanline %lu, pixel %lu, sample %ld: ",
			    (long) row, (long) pixel, (long) sample);
			if (reversed)
				printf("%02x %02x\n", *p2, *cp1);
			else
				printf("%02x %02x\n", *cp1, *p2);
			if (--stopondiff == 0)
				exit(1);
		}
	}
	return 0;
}
