static int JBIGDecode(TIFF* tif, uint8* buffer, tmsize_t size, uint16 s)
{
	struct jbg_dec_state decoder;
	int decodeStatus = 0;
	unsigned char* pImage = NULL;
	(void) size, (void) s;
	if (isFillOrder(tif, tif->tif_dir.td_fillorder))
	{
		TIFFReverseBits(tif->tif_rawdata, tif->tif_rawdatasize);
	}
	jbg_dec_init(&decoder);
#if defined(HAVE_JBG_NEWLEN)
	jbg_newlen(tif->tif_rawdata, (size_t)tif->tif_rawdatasize);
	









#endif 
	decodeStatus = jbg_dec_in(&decoder, (unsigned char*)tif->tif_rawdata,
				  (size_t)tif->tif_rawdatasize, NULL);
	if (JBG_EOK != decodeStatus)
	{
		




		TIFFErrorExt(tif->tif_clientdata,
			     "JBIG", "Error (%d) decoding: %s",
			     decodeStatus,
#if defined(JBG_EN)
			     jbg_strerror(decodeStatus, JBG_EN)
#else
			     jbg_strerror(decodeStatus)
#endif
			     );
		return 0;
	}
	pImage = jbg_dec_getimage(&decoder, 0);
	_TIFFmemcpy(buffer, pImage, jbg_dec_getsize(&decoder));
	jbg_dec_free(&decoder);
	return 1;
}
