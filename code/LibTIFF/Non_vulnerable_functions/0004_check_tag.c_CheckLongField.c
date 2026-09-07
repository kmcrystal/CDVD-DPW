int
CheckLongField(TIFF *tif, const ttag_t field, const uint32 value)
{
	uint32 tmp = 123;
	if (!TIFFGetField(tif, field, &tmp)) {
		fprintf (stderr, "Problem fetching tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	if (tmp != value) {
		fprintf (stderr, "Wrong LONG value fetched for tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	return 0;
}
