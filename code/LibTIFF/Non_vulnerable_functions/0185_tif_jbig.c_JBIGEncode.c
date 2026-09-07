static int JBIGEncode(TIFF* tif, uint8* buffer, tmsize_t size, uint16 s)
{
	TIFFDirectory* dir = &tif->tif_dir;
	struct jbg_enc_state encoder;
	(void) size, (void) s;
	jbg_enc_init(&encoder,
		     dir->td_imagewidth,
		     dir->td_imagelength,
		     1,
		     &buffer,
		     JBIGOutputBie,
		     tif);
	



	jbg_enc_out(&encoder);
	jbg_enc_free(&encoder);
	return 1;
}
