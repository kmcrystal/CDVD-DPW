int
TIFFDefaultDirectory(TIFF* tif)
{
	register TIFFDirectory* td = &tif->tif_dir;
	size_t tiffFieldInfoCount;
	const TIFFFieldInfo *tiffFieldInfo =
		_TIFFGetFieldInfo(&tiffFieldInfoCount);
	_TIFFSetupFieldInfo(tif, tiffFieldInfo, tiffFieldInfoCount);
	_TIFFmemset(td, 0, sizeof (*td));
	td->td_fillorder = FILLORDER_MSB2LSB;
	td->td_bitspersample = 1;
	td->td_threshholding = THRESHHOLD_BILEVEL;
	td->td_orientation = ORIENTATION_TOPLEFT;
	td->td_samplesperpixel = 1;
	td->td_rowsperstrip = (uint32) -1;
	td->td_tilewidth = 0;
	td->td_tilelength = 0;
	td->td_tiledepth = 1;
	td->td_stripbytecountsorted = 1; 
	td->td_resolutionunit = RESUNIT_INCH;
	td->td_sampleformat = SAMPLEFORMAT_UINT;
	td->td_imagedepth = 1;
	td->td_ycbcrsubsampling[0] = 2;
	td->td_ycbcrsubsampling[1] = 2;
	td->td_ycbcrpositioning = YCBCRPOSITION_CENTERED;
	tif->tif_postdecode = _TIFFNoPostDecode;
        tif->tif_foundfield = NULL;
	tif->tif_tagmethods.vsetfield = _TIFFVSetField;
	tif->tif_tagmethods.vgetfield = _TIFFVGetField;
	tif->tif_tagmethods.printdir = NULL;
	



	if (_TIFFextender)
		(*_TIFFextender)(tif);
	(void) TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	







	tif->tif_flags &= ~TIFF_DIRTYDIRECT;
        




        tif->tif_flags &= ~TIFF_ISTILED;
	return (1);
}
