static void
Fax3PrematureEOF(const char* module, TIFF* tif, uint32 line, uint32 a0)
{
	TIFFWarningExt(tif->tif_clientdata, module, "%s: Premature EOF at line %lu of %s %lu (x %lu)",
	    tif->tif_name,
	    (unsigned long) line, isTiled(tif) ? "tile" : "strip",
        (unsigned long) (isTiled(tif) ? tif->tif_curtile : tif->tif_curstrip),
        (unsigned long) a0);
}
