static void
quant_fsdither(TIFF* in, TIFF* out)
{
	unsigned char *outline, *inputline, *inptr;
	short *thisline, *nextline;
	register unsigned char	*outptr;
	register short *thisptr, *nextptr;
	register uint32 i, j;
	uint32 imax, jmax;
	int lastline, lastpixel;
	imax = imagelength - 1;
	jmax = imagewidth - 1;
	inputline = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(in));
	thisline = (short *)_TIFFmalloc(imagewidth * 3 * sizeof (short));
	nextline = (short *)_TIFFmalloc(imagewidth * 3 * sizeof (short));
	outline = (unsigned char *) _TIFFmalloc(TIFFScanlineSize(out));
	GetInputLine(in, 0, goto bad);		
	for (i = 1; i <= imagelength; ++i) {
		SWAP(short *, thisline, nextline);
		lastline = (i >= imax);
		if (i <= imax)
			GetInputLine(in, i, break);
		thisptr = thisline;
		nextptr = nextline;
		outptr = outline;
		for (j = 0; j < imagewidth; ++j) {
			int red, green, blue;
			register int oval, r2, g2, b2;
			lastpixel = (j == jmax);
			GetComponent(*thisptr++, r2, red);
			GetComponent(*thisptr++, g2, green);
			GetComponent(*thisptr++, b2, blue);
			oval = histogram[r2][g2][b2];
			if (oval == -1) {
				int ci;
				register int cj, tmp, d2, dist;
				register C_cell	*cell;
				cell = *(ColorCells +
				    (((r2>>(B_DEPTH-C_DEPTH)) << C_DEPTH*2) +
				    ((g2>>(B_DEPTH-C_DEPTH)) << C_DEPTH ) +
				    (b2>>(B_DEPTH-C_DEPTH))));
				if (cell == NULL)
					cell = create_colorcell(red,
					    green, blue);
				dist = 9999999;
				for (ci = 0; ci < cell->num_ents && dist > cell->entries[ci][1]; ++ci) {
					cj = cell->entries[ci][0];
					d2 = (rm[cj] >> COLOR_SHIFT) - r2;
					d2 *= d2;
					tmp = (gm[cj] >> COLOR_SHIFT) - g2;
					d2 += tmp*tmp;
					tmp = (bm[cj] >> COLOR_SHIFT) - b2;
					d2 += tmp*tmp;
					if (d2 < dist) {
						dist = d2;
						oval = cj;
					}
				}
				histogram[r2][g2][b2] = oval;
			}
			*outptr++ = oval;
			red -= rm[oval];
			green -= gm[oval];
			blue -= bm[oval];
			if (!lastpixel) {
				thisptr[0] += blue * 7 / 16;
				thisptr[1] += green * 7 / 16;
				thisptr[2] += red * 7 / 16;
			}
			if (!lastline) {
				if (j != 0) {
					nextptr[-3] += blue * 3 / 16;
					nextptr[-2] += green * 3 / 16;
					nextptr[-1] += red * 3 / 16;
				}
				nextptr[0] += blue * 5 / 16;
				nextptr[1] += green * 5 / 16;
				nextptr[2] += red * 5 / 16;
				if (!lastpixel) {
					nextptr[3] += blue / 16;
				        nextptr[4] += green / 16;
				        nextptr[5] += red / 16;
				}
				nextptr += 3;
			}
		}
		if (TIFFWriteScanline(out, outline, i-1, 0) < 0)
			break;
	}
bad:
	_TIFFfree(inputline);
	_TIFFfree(thisline);
	_TIFFfree(nextline);
	_TIFFfree(outline);
}
