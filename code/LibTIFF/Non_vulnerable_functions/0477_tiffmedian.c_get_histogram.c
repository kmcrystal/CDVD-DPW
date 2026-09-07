static void
get_histogram(TIFF* in, Colorbox* box)
{
	register unsigned char *inptr;
	register int red, green, blue;
	register uint32 j, i;
	unsigned char *inputline;
	inputline = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(in));
	if (inputline == NULL) {
		fprintf(stderr, "No space for scanline buffer\n");
		exit(-1);
	}
	box->rmin = box->gmin = box->bmin = 999;
	box->rmax = box->gmax = box->bmax = -1;
	box->total = imagewidth * imagelength;
	{ register uint32 *ptr = &histogram[0][0][0];
	  for (i = B_LEN*B_LEN*B_LEN; i-- > 0;)
		*ptr++ = 0;
	}
	for (i = 0; i < imagelength; i++) {
		if (TIFFReadScanline(in, inputline, i, 0) <= 0)
			break;
		inptr = inputline;
		for (j = imagewidth; j-- > 0;) {
			red = (*inptr++) & 0xff >> COLOR_SHIFT;
			green = (*inptr++) & 0xff >> COLOR_SHIFT;
			blue = (*inptr++) & 0xff >> COLOR_SHIFT;
                        if ((red | green | blue) >= B_LEN) {
                                fprintf(stderr,
                                        "Logic error. "
                                        "Histogram array overflow!\n");
                                exit(-6);
                        }
			if (red < box->rmin)
				box->rmin = red;
		        if (red > box->rmax)
				box->rmax = red;
		        if (green < box->gmin)
				box->gmin = green;
		        if (green > box->gmax)
				box->gmax = green;
		        if (blue < box->bmin)
				box->bmin = blue;
		        if (blue > box->bmax)
				box->bmax = blue;
		        histogram[red][green][blue]++;
		}
	}
	_TIFFfree(inputline);
}
