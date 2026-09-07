static int
PixarLogSetupDecode(TIFF* tif)
{
	static const char module[] = "PixarLogSetupDecode";
	TIFFDirectory *td = &tif->tif_dir;
	PixarLogState* sp = DecoderState(tif);
	tmsize_t tbuf_size;
	assert(sp != NULL);
	
	
	
	if( (sp->state & PLSTATE_INIT) != 0 )
		return 1;
	

	tif->tif_postdecode = _TIFFNoPostDecode;  
	
	sp->stride = (td->td_planarconfig == PLANARCONFIG_CONTIG ?
	    td->td_samplesperpixel : 1);
	tbuf_size = multiply_ms(multiply_ms(multiply_ms(sp->stride, td->td_imagewidth),
				      td->td_rowsperstrip), sizeof(uint16));
	
	tbuf_size = add_ms(tbuf_size, sizeof(uint16) * sp->stride);
	if (tbuf_size == 0)
		return (0);   
	sp->tbuf = (uint16 *) _TIFFmalloc(tbuf_size);
	if (sp->tbuf == NULL)
		return (0);
	sp->tbuf_size = tbuf_size;
	if (sp->user_datafmt == PIXARLOGDATAFMT_UNKNOWN)
		sp->user_datafmt = PixarLogGuessDataFmt(td);
	if (sp->user_datafmt == PIXARLOGDATAFMT_UNKNOWN) {
                _TIFFfree(sp->tbuf);
                sp->tbuf = NULL;
                sp->tbuf_size = 0;
		TIFFErrorExt(tif->tif_clientdata, module,
			"PixarLog compression can't handle bits depth/data format combination (depth: %d)", 
			td->td_bitspersample);
		return (0);
	}
	if (inflateInit(&sp->stream) != Z_OK) {
                _TIFFfree(sp->tbuf);
                sp->tbuf = NULL;
                sp->tbuf_size = 0;
		TIFFErrorExt(tif->tif_clientdata, module, "%s", sp->stream.msg ? sp->stream.msg : "(null)");
		return (0);
	} else {
		sp->state |= PLSTATE_INIT;
		return (1);
	}
}
