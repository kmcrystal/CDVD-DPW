void*
_TIFFCheckMalloc(TIFF* tif, tmsize_t nmemb, tmsize_t elem_size, const char* what)
{
	return _TIFFCheckRealloc(tif, NULL, nmemb, elem_size, what);  
}
