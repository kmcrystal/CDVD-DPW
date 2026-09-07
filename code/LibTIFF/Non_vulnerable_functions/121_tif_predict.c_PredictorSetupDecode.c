static int
PredictorSetupDecode(TIFF* tif)
{
	TIFFPredictorState* sp = PredictorState(tif);
	TIFFDirectory* td = &tif->tif_dir;
	
	
	
	if (!(*sp->setupdecode)(tif) || !PredictorSetup(tif))
		return 0;
	if (sp->predictor == 2) {
		switch (td->td_bitspersample) {
			case 8:  sp->decodepfunc = horAcc8; break;
			case 16: sp->decodepfunc = horAcc16; break;
			case 32: sp->decodepfunc = horAcc32; break;
		}
		



                if( tif->tif_decoderow != PredictorDecodeRow )
                {
                    sp->decoderow = tif->tif_decoderow;
                    tif->tif_decoderow = PredictorDecodeRow;
                    sp->decodestrip = tif->tif_decodestrip;
                    tif->tif_decodestrip = PredictorDecodeTile;
                    sp->decodetile = tif->tif_decodetile;
                    tif->tif_decodetile = PredictorDecodeTile;
                }
		






		if (tif->tif_flags & TIFF_SWAB) {
			if (sp->decodepfunc == horAcc16) {
				sp->decodepfunc = swabHorAcc16;
				tif->tif_postdecode = _TIFFNoPostDecode;
            } else if (sp->decodepfunc == horAcc32) {
				sp->decodepfunc = swabHorAcc32;
				tif->tif_postdecode = _TIFFNoPostDecode;
            }
		}
	}
	else if (sp->predictor == 3) {
		sp->decodepfunc = fpAcc;
		



                if( tif->tif_decoderow != PredictorDecodeRow )
                {
                    sp->decoderow = tif->tif_decoderow;
                    tif->tif_decoderow = PredictorDecodeRow;
                    sp->decodestrip = tif->tif_decodestrip;
                    tif->tif_decodestrip = PredictorDecodeTile;
                    sp->decodetile = tif->tif_decodetile;
                    tif->tif_decodetile = PredictorDecodeTile;
                }
		




		if (tif->tif_flags & TIFF_SWAB) {
			tif->tif_postdecode = _TIFFNoPostDecode;
		}
		



	}
	return 1;
}
