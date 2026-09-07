static int
_notConfigured(TIFF* tif)
{
	const TIFFCodec* c = TIFFFindCODEC(tif->tif_dir.td_compression);
        char compression_code[20];
        sprintf(compression_code, "%d",tif->tif_dir.td_compression );
	TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
                     "%s compression support is not configured", 
                     c ? c->name : compression_code );
	return (0);
}
