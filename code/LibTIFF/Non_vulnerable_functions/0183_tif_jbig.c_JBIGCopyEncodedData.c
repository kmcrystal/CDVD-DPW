static int JBIGCopyEncodedData(TIFF* tif, unsigned char* pp, size_t cc, uint16 s)
{
	(void) s;
	while (cc > 0)
	{
		tmsize_t n = (tmsize_t)cc;
		if (tif->tif_rawcc + n > tif->tif_rawdatasize)
		{
			n = tif->tif_rawdatasize - tif->tif_rawcc;
		}
		assert(n > 0);
		_TIFFmemcpy(tif->tif_rawcp, pp, n);
		tif->tif_rawcp += n;
		tif->tif_rawcc += n;
		pp += n;
		cc -= (size_t)n;
		if (tif->tif_rawcc >= tif->tif_rawdatasize &&
		    !TIFFFlushData1(tif))
		{
			return (-1);
		}
	}
	return (1);
}
