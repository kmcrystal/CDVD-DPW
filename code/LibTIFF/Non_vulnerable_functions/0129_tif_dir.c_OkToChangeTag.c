static int
OkToChangeTag(TIFF* tif, ttag_t tag)
{
	const TIFFFieldInfo* fip = _TIFFFindFieldInfo(tif, tag, TIFF_ANY);
	if (!fip) {			
		TIFFErrorExt(tif->tif_clientdata, "TIFFSetField", "%s: Unknown %stag %u",
		    tif->tif_name, isPseudoTag(tag) ? "pseudo-" : "", tag);
		return (0);
	}
	if (tag != TIFFTAG_IMAGELENGTH && (tif->tif_flags & TIFF_BEENWRITING) &&
	    !fip->field_oktochange) {
		





		TIFFErrorExt(tif->tif_clientdata, "TIFFSetField",
		    "%s: Cannot modify tag \"%s\" while writing",
		    tif->tif_name, fip->field_name);
		return (0);
	}
	return (1);
}
