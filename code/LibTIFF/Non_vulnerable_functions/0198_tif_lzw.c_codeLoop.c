static void
codeLoop(TIFF* tif, const char* module)
{
	TIFFErrorExt(tif->tif_clientdata, module,
	    "Bogus encoding, loop in the code table; scanline %d",
	    tif->tif_row);
}
