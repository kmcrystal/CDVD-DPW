static int
PredictorSetupEncode(TIFF* tif)
{
	TIFFPredictorState* sp = PredictorState(tif);
	TIFFDirectory* td = &tif->tif_dir;
	if (!(*sp->setupencode)(tif) || !PredictorSetup(tif))
		return 0;
	if (sp->predictor == 2) {
		switch (td->td_bitspersample) {
			case 8:  sp->encodepfunc = horDiff8; break;
			case 16: sp->encodepfunc = horDiff16; break;
			case 32: sp->encodepfunc = horDiff32; break;
		}
		



                if( tif->tif_encoderow != PredictorEncodeRow )
                {
                    sp->encoderow = tif->tif_encoderow;
                    tif->tif_encoderow = PredictorEncodeRow;
                    sp->encodestrip = tif->tif_encodestrip;
                    tif->tif_encodestrip = PredictorEncodeTile;
                    sp->encodetile = tif->tif_encodetile;
                    tif->tif_encodetile = PredictorEncodeTile;
                }
                






                if (tif->tif_flags & TIFF_SWAB) {
                    if (sp->encodepfunc == horDiff16) {
                            sp->encodepfunc = swabHorDiff16;
                            tif->tif_postdecode = _TIFFNoPostDecode;
                    } else if (sp->encodepfunc == horDiff32) {
                            sp->encodepfunc = swabHorDiff32;
                            tif->tif_postdecode = _TIFFNoPostDecode;
                    }
                }
        }
	else if (sp->predictor == 3) {
		sp->encodepfunc = fpDiff;
		



                if( tif->tif_encoderow != PredictorEncodeRow )
                {
                    sp->encoderow = tif->tif_encoderow;
                    tif->tif_encoderow = PredictorEncodeRow;
                    sp->encodestrip = tif->tif_encodestrip;
                    tif->tif_encodestrip = PredictorEncodeTile;
                    sp->encodetile = tif->tif_encodetile;
                    tif->tif_encodetile = PredictorEncodeTile;
                }
	}
	return 1;
}
