int TIFFInitJBIG(TIFF* tif, int scheme)
{
	assert(scheme == COMPRESSION_JBIG);
	




	tif->tif_flags |= TIFF_NOBITREV;
	tif->tif_flags &= ~TIFF_MAPPED;
	
	tif->tif_setupdecode = JBIGSetupDecode;
	tif->tif_decodestrip = JBIGDecode;
	tif->tif_setupencode = JBIGSetupEncode;
	tif->tif_encodestrip = JBIGEncode;
	return 1;
}
