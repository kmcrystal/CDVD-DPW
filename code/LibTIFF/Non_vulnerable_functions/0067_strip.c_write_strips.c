int
write_strips(TIFF *tif, const tdata_t array, const tsize_t size)
{
	tstrip_t	strip, nstrips;
	tsize_t		stripsize, offset;
	stripsize = TIFFStripSize(tif);
	if (!stripsize) {
		fprintf (stderr, "Wrong size of strip.\n");
		return -1;
	}
	nstrips = TIFFNumberOfStrips(tif);
	for (offset = 0, strip = 0;
	     offset < size && strip < nstrips;
	     offset+=stripsize, strip++) {
		


		tsize_t	bufsize = size - offset;
		if (bufsize > stripsize)
			bufsize = stripsize;
		if (TIFFWriteEncodedStrip(tif, strip, (char *)array + offset,
					  bufsize) != bufsize) {
			fprintf (stderr, "Can't write strip %lu.\n",
				 (unsigned long)strip);
			return -1;
		}
        }
	return 0;
}
