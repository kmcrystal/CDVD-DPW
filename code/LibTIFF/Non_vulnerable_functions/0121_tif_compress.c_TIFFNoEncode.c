static int
TIFFNoEncode(TIFF* tif, const char* method)
{
	const TIFFCodec* c = TIFFFindCODEC(tif->tif_dir.td_compression);
	if (c) { 
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
			     "%s %s encoding is not implemented",
			     c->name, method);
	} else { 
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
			"Compression scheme %u %s encoding is not implemented",
			     tif->tif_dir.td_compression, method);
	}
	return (-1);
}
