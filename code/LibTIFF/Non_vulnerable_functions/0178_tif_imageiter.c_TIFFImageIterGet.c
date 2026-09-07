int
TIFFImageIterGet(TIFFImageIter* img, void *udata, uint32 w, uint32 h)
{
    if (img->get == NULL) {
	TIFFErrorExt(img->tif->tif_clientdata, TIFFFileName(img->tif), "No \"get\" routine setup");
	return (0);
    }
    if (img->callback.any == NULL) {
	TIFFErrorExt(img->tif->tif_clientdata, TIFFFileName(img->tif),
		"No \"put\" routine setupl; probably can not handle image format");
	return (0);
    }
    return (*img->get)(img, udata, w, h);
}
