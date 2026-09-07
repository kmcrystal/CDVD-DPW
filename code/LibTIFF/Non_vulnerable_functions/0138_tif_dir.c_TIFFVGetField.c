int
TIFFVGetField(TIFF* tif, ttag_t tag, va_list ap)
{
	const TIFFFieldInfo* fip = _TIFFFindFieldInfo(tif, tag, TIFF_ANY);
	return (fip && (isPseudoTag(tag) || TIFFFieldSet(tif, fip->field_bit)) ?
	    (*tif->tif_tagmethods.vgetfield)(tif, tag, ap) : 0);
}
