int
TIFFPredictorCleanup(TIFF* tif)
{
	TIFFPredictorState* sp = PredictorState(tif);
	assert(sp != 0);
	tif->tif_tagmethods.vgetfield = sp->vgetparent;
	tif->tif_tagmethods.vsetfield = sp->vsetparent;
	tif->tif_tagmethods.printdir = sp->printdir;
	tif->tif_setupdecode = sp->setupdecode;
	tif->tif_setupencode = sp->setupencode;
	return 1;
}
