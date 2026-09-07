static void
MissingRequired(TIFF* tif, const char* tagname)
{
	TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
	    "TIFF directory is missing required \"%s\" field", tagname);
}
