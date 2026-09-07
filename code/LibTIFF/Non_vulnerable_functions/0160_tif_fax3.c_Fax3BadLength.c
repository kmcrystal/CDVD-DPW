static void
Fax3BadLength(const char* module, TIFF* tif, uint32 line, uint32 a0, uint32 lastx)
{
	TIFFWarningExt(tif->tif_clientdata, module, "%s: %s at line %lu of %s %lu (got %lu, expected %lu)",
	    tif->tif_name,
	    a0 < lastx ? "Premature EOL" : "Line length mismatch",
	    (unsigned long) line, isTiled(tif) ? "tile" : "strip",
        (unsigned long) (isTiled(tif) ? tif->tif_curtile : tif->tif_curstrip),
        (unsigned long) a0, lastx);
}
