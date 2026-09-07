void
XTIFFClose(TIFF *tif)
{
	xtiff *xt = XTIFFDIR(tif);
	
	TIFFClose(tif);
	
	_XTIFFFreeDirectory(xt);
	_TIFFfree(xt);
}
