static int
PredictorVSetField(TIFF* tif, uint32 tag, va_list ap)
{
	TIFFPredictorState *sp = PredictorState(tif);
	assert(sp != NULL);
	assert(sp->vsetparent != NULL);
	switch (tag) {
	case TIFFTAG_PREDICTOR:
		sp->predictor = (uint16) va_arg(ap, uint16_vap);
		TIFFSetFieldBit(tif, FIELD_PREDICTOR);
		break;
	default:
		return (*sp->vsetparent)(tif, tag, ap);
	}
	tif->tif_flags |= TIFF_DIRTYDIRECT;
	return 1;
}
