int
CheckShortPairedField(TIFF *tif, const ttag_t field, const uint16 *values)
{
	uint16 tmp[2] = { 123, 456 };
	if (!TIFFGetField(tif, field, tmp, tmp + 1)) {
		fprintf (stderr, "Problem fetching tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	if (tmp[0] != values[0] || tmp[1] != values[1]) {
		fprintf (stderr, "Wrong SHORT PAIR fetched for tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	return 0;
}
