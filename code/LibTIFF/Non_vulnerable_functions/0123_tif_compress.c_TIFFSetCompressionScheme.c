int
TIFFSetCompressionScheme(TIFF* tif, int scheme)
{
	const TIFFCodec *c = TIFFFindCODEC((uint16) scheme);
	_TIFFSetDefaultCompressionState(tif);
	





	return (c ? (*c->init)(tif, scheme) : 1);
}
