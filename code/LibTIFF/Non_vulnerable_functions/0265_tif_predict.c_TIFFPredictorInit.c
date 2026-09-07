int
TIFFPredictorInit(TIFF* tif)
{
	TIFFPredictorState* sp = PredictorState(tif);
	assert(sp != 0);
	


	if (!_TIFFMergeFields(tif, predictFields,
			      TIFFArrayCount(predictFields))) {
		TIFFErrorExt(tif->tif_clientdata, "TIFFPredictorInit",
		    "Merging Predictor codec-specific tags failed");
		return 0;
	}
	


	sp->vgetparent = tif->tif_tagmethods.vgetfield;
	tif->tif_tagmethods.vgetfield =
            PredictorVGetField;
	sp->vsetparent = tif->tif_tagmethods.vsetfield;
	tif->tif_tagmethods.vsetfield =
	    PredictorVSetField;
	sp->printdir = tif->tif_tagmethods.printdir;
	tif->tif_tagmethods.printdir =
            PredictorPrintDir;	
	sp->setupdecode = tif->tif_setupdecode;
	tif->tif_setupdecode = PredictorSetupDecode;
	sp->setupencode = tif->tif_setupencode;
	tif->tif_setupencode = PredictorSetupEncode;
	sp->predictor = 1;			
	sp->encodepfunc = NULL;			
	sp->decodepfunc = NULL;			
	return 1;
}
