int
read_strips(TIFF *tif, const tdata_t array, const tsize_t size)
{
	tstrip_t	strip, nstrips;
	tsize_t		stripsize, offset;
	tdata_t		buf = NULL;
	stripsize = TIFFStripSize(tif);
	if (!stripsize) {
		fprintf (stderr, "Wrong size of strip.\n");
		return -1;
	}
	buf = _TIFFmalloc(stripsize);
	if (!buf) {
		fprintf (stderr, "Can't allocate space for strip buffer.\n");
		return -1;
	}
	nstrips = TIFFNumberOfStrips(tif);
	for (offset = 0, strip = 0;
	     offset < size && strip < nstrips;
	     offset+=stripsize, strip++) {
		


		tsize_t	bufsize = size - offset;
		if (bufsize > stripsize)
			bufsize = stripsize;
		if (TIFFReadEncodedStrip(tif, strip, buf, -1) != bufsize) {
			fprintf (stderr, "Can't read strip %lu.\n",
				 (unsigned long)strip);
			return -1;
		}
		if (memcmp(buf, (char *)array + offset, bufsize) != 0) {
			fprintf (stderr, "Wrong data read for strip %lu.\n",
				 (unsigned long)strip);
			_TIFFfree(buf);
			return -1;
		}
        }
	_TIFFfree(buf);
	return 0;
}
