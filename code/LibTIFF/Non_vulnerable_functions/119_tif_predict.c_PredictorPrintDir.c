static void
PredictorPrintDir(TIFF* tif, FILE* fd, long flags)
{
	TIFFPredictorState* sp = PredictorState(tif);
	(void) flags;
	if (TIFFFieldSet(tif,FIELD_PREDICTOR)) {
		fprintf(fd, "  Predictor: ");
		switch (sp->predictor) {
			case 1: fprintf(fd, "none "); break;
			case 2: fprintf(fd, "horizontal differencing "); break;
			case 3: fprintf(fd, "floating point predictor "); break;
		}
		fprintf(fd, "%d (0x%x)\n", sp->predictor, sp->predictor);
	}
	if (sp->printdir)
		(*sp->printdir)(tif, fd, flags);
}
