static int
PredictorVGetField(TIFF* tif, uint32 tag, va_list ap)
{
	TIFFPredictorState *sp = PredictorState(tif);
	assert(sp != NULL);
	assert(sp->vgetparent != NULL);
	switch (tag) {
	case TIFFTAG_PREDICTOR:
		*va_arg(ap, uint16*) = (uint16)sp->predictor;
		break;
	default:
		return (*sp->vgetparent)(tif, tag, ap);
	}
	return 1;
}
