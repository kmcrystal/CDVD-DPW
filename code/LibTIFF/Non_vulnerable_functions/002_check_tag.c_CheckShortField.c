int
CheckShortField(TIFF *tif, const ttag_t field, const uint16 value)
{
	uint16 tmp = 123;
	if (!TIFFGetField(tif, field, &tmp)) {
		fprintf (stderr, "Problem fetching tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	if (tmp != value) {
		fprintf (stderr, "Wrong SHORT value fetched for tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	return 0;
}
