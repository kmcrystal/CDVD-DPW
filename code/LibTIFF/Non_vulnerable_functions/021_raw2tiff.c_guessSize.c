static int
guessSize(int fd, TIFFDataType dtype, off_t hdr_size, uint32 nbands,
	  int swab, uint32 *width, uint32 *length)
{
	const float longt = 40.0;    
	char	    *buf1, *buf2;
	struct stat filestat;
	uint32	    w, h, scanlinesize, imagesize;
	uint32	    depth = TIFFDataWidth(dtype);
	float	    cor_coef = 0, tmp;
	fstat(fd, &filestat);
	if (filestat.st_size < hdr_size) {
		fprintf(stderr, "Too large header size specified.\n");
		return -1;
	}
	imagesize = (filestat.st_size - hdr_size) / nbands / depth;
	if (*width != 0 && *length == 0) {
		fprintf(stderr,	"Image height is not specified.\n");
		*length = imagesize / *width;
		fprintf(stderr, "Height is guessed as %lu.\n",
			(unsigned long)*length);
		return 1;
	} else if (*width == 0 && *length != 0) {
		fprintf(stderr, "Image width is not specified.\n");
		*width = imagesize / *length;
		fprintf(stderr,	"Width is guessed as %lu.\n",
			(unsigned long)*width);
		return 1;
	} else if (*width == 0 && *length == 0) {
		fprintf(stderr,	"Image width and height are not specified.\n");
		for (w = (uint32) sqrt(imagesize / longt);
		     w < sqrt(imagesize * longt);
		     w++) {
			if (imagesize % w == 0) {
				scanlinesize = w * depth;
				buf1 = _TIFFmalloc(scanlinesize);
				buf2 = _TIFFmalloc(scanlinesize);
				h = imagesize / w;
				lseek(fd, hdr_size + (int)(h/2)*scanlinesize,
				      SEEK_SET);
				read(fd, buf1, scanlinesize);
				read(fd, buf2, scanlinesize);
				if (swab) {
					swapBytesInScanline(buf1, w, dtype);
					swapBytesInScanline(buf2, w, dtype);
				}
				tmp = (float) fabs(correlation(buf1, buf2,
							       w, dtype));
				if (tmp > cor_coef) {
					cor_coef = tmp;
					*width = w, *length = h;
				}
				_TIFFfree(buf1);
				_TIFFfree(buf2);
			}
		}
		fprintf(stderr,
			"Width is guessed as %lu, height is guessed as %lu.\n",
			(unsigned long)*width, (unsigned long)*length);
		return 1;
	} else {
		if (filestat.st_size<(off_t)(hdr_size+(*width)*(*length)*nbands*depth)) {
			fprintf(stderr, "Input file too small.\n");
		return -1;
		}
	}
	return 1;
}
