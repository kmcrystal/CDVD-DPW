int
TIFFVSetField(TIFF* tif, ttag_t tag, va_list ap)
{
	return OkToChangeTag(tif, tag) ?
	    (*tif->tif_tagmethods.vsetfield)(tif, tag, ap) : 0;
}
