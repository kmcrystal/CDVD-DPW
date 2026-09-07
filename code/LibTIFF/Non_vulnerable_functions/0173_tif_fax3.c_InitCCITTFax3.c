static int
InitCCITTFax3(TIFF* tif)
{
	Fax3BaseState* sp;
	


	tif->tif_data = (tidata_t)
		_TIFFmalloc(sizeof (Fax3CodecState));
	if (tif->tif_data == NULL) {
		TIFFErrorExt(tif->tif_clientdata, "TIFFInitCCITTFax3",
		    "%s: No space for state block", tif->tif_name);
		return (0);
	}
	sp = Fax3State(tif);
        sp->rw_mode = tif->tif_mode;
	



	_TIFFMergeFieldInfo(tif, faxFieldInfo, N(faxFieldInfo));
	sp->vgetparent = tif->tif_tagmethods.vgetfield;
	tif->tif_tagmethods.vgetfield = Fax3VGetField; 
	sp->vsetparent = tif->tif_tagmethods.vsetfield;
	tif->tif_tagmethods.vsetfield = Fax3VSetField; 
	tif->tif_tagmethods.printdir = Fax3PrintDir;   
	sp->groupoptions = 0;	
	sp->recvparams = 0;
	sp->subaddress = NULL;
	sp->faxdcs = NULL;
	if (sp->rw_mode == O_RDONLY) 
		tif->tif_flags |= TIFF_NOBITREV; 
	DecoderState(tif)->runs = NULL;
	TIFFSetField(tif, TIFFTAG_FAXFILLFUNC, _TIFFFax3fillruns);
	EncoderState(tif)->refline = NULL;
	


	tif->tif_setupdecode = Fax3SetupState;
	tif->tif_predecode = Fax3PreDecode;
	tif->tif_decoderow = Fax3Decode1D;
	tif->tif_decodestrip = Fax3Decode1D;
	tif->tif_decodetile = Fax3Decode1D;
	tif->tif_setupencode = Fax3SetupState;
	tif->tif_preencode = Fax3PreEncode;
	tif->tif_postencode = Fax3PostEncode;
	tif->tif_encoderow = Fax3Encode;
	tif->tif_encodestrip = Fax3Encode;
	tif->tif_encodetile = Fax3Encode;
	tif->tif_close = Fax3Close;
	tif->tif_cleanup = Fax3Cleanup;
	return (1);
}
