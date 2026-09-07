static char *
CheckMalloc(TIFF* tif, tsize_t n, const char* what)
{
	char *cp = (char*)_TIFFmalloc(n);
	if (cp == NULL)
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "No space %s", what);
	return (cp);
}
