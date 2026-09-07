static int
PixarLogVSetField(TIFF* tif, uint32 tag, va_list ap)
{
    static const char module[] = "PixarLogVSetField";
    PixarLogState *sp = (PixarLogState *)tif->tif_data;
    int result;
    switch (tag) {
     case TIFFTAG_PIXARLOGQUALITY:
		sp->quality = (int) va_arg(ap, int);
		if (tif->tif_mode != O_RDONLY && (sp->state&PLSTATE_INIT)) {
			if (deflateParams(&sp->stream,
			    sp->quality, Z_DEFAULT_STRATEGY) != Z_OK) {
				TIFFErrorExt(tif->tif_clientdata, module, "ZLib error: %s",
					sp->stream.msg ? sp->stream.msg : "(null)");
				return (0);
			}
		}
		return (1);
     case TIFFTAG_PIXARLOGDATAFMT:
	sp->user_datafmt = (int) va_arg(ap, int);
	




	switch (sp->user_datafmt) {
	 case PIXARLOGDATAFMT_8BIT:
	 case PIXARLOGDATAFMT_8BITABGR:
	    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
	    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	    break;
	 case PIXARLOGDATAFMT_11BITLOG:
	    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
	    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	    break;
	 case PIXARLOGDATAFMT_12BITPICIO:
	    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
	    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
	    break;
	 case PIXARLOGDATAFMT_16BIT:
	    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
	    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	    break;
	 case PIXARLOGDATAFMT_FLOAT:
	    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
	    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
	    break;
	}
	


	tif->tif_tilesize = isTiled(tif) ? TIFFTileSize(tif) : (tmsize_t)(-1);
	tif->tif_scanlinesize = TIFFScanlineSize(tif);
	result = 1;		
	break;
     default:
	result = (*sp->vsetparent)(tif, tag, ap);
    }
    return (result);
}
