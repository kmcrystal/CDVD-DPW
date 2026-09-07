static void
Fax3Extension(const char* module, TIFF* tif, uint32 line, uint32 a0)
{
	TIFFErrorExt(tif->tif_clientdata, module,
	    "%s: Uncompressed data (not supported) at line %lu of %s %lu (x %lu)",
	    tif->tif_name, (unsigned long) line, isTiled(tif) ? "tile" : "strip",
       (unsigned long) (isTiled(tif) ? tif->tif_curtile : tif->tif_curstrip),
       (unsigned long) a0);
}
