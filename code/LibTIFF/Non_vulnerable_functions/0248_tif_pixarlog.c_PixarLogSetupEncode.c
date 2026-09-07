static int
PixarLogSetupEncode(TIFF* tif)
{
	static const char module[] = "PixarLogSetupEncode";
	TIFFDirectory *td = &tif->tif_dir;
	PixarLogState* sp = EncoderState(tif);
	tmsize_t tbuf_size;
	assert(sp != NULL);
	
	sp->stride = (td->td_planarconfig == PLANARCONFIG_CONTIG ?
	    td->td_samplesperpixel : 1);
	tbuf_size = multiply_ms(multiply_ms(multiply_ms(sp->stride, td->td_imagewidth),
				      td->td_rowsperstrip), sizeof(uint16));
	if (tbuf_size == 0)
		return (0);  
	sp->tbuf = (uint16 *) _TIFFmalloc(tbuf_size);
	if (sp->tbuf == NULL)
		return (0);
	if (sp->user_datafmt == PIXARLOGDATAFMT_UNKNOWN)
		sp->user_datafmt = PixarLogGuessDataFmt(td);
	if (sp->user_datafmt == PIXARLOGDATAFMT_UNKNOWN) {
		TIFFErrorExt(tif->tif_clientdata, module, "PixarLog compression can't handle %d bit linear encodings", td->td_bitspersample);
		return (0);
	}
	if (deflateInit(&sp->stream, sp->quality) != Z_OK) {
		TIFFErrorExt(tif->tif_clientdata, module, "%s", sp->stream.msg ? sp->stream.msg : "(null)");
		return (0);
	} else {
		sp->state |= PLSTATE_INIT;
		return (1);
	}
}
