static void
quant(TIFF* in, TIFF* out)
{
	unsigned char	*outline, *inputline;
	register unsigned char	*outptr, *inptr;
	register uint32 i, j;
	register int red, green, blue;
	inputline = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(in));
	outline = (unsigned char *)_TIFFmalloc(imagewidth);
	for (i = 0; i < imagelength; i++) {
		if (TIFFReadScanline(in, inputline, i, 0) <= 0)
			break;
		inptr = inputline;
		outptr = outline;
		for (j = 0; j < imagewidth; j++) {
			red = *inptr++ >> COLOR_SHIFT;
			green = *inptr++ >> COLOR_SHIFT;
			blue = *inptr++ >> COLOR_SHIFT;
			*outptr++ = (unsigned char)histogram[red][green][blue];
		}
		if (TIFFWriteScanline(out, outline, i, 0) < 0)
			break;
	}
	_TIFFfree(inputline);
	_TIFFfree(outline);
}
