int
_TIFFNoTileDecode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) pp; (void) cc; (void) s;
	return (TIFFNoDecode(tif, "tile"));
}
