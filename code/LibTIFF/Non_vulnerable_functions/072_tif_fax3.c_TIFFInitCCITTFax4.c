int
TIFFInitCCITTFax4(TIFF* tif, int scheme)
{
	(void) scheme;
	if (InitCCITTFax3(tif)) {		
		_TIFFMergeFieldInfo(tif, fax4FieldInfo, N(fax4FieldInfo));
		tif->tif_decoderow = Fax4Decode;
		tif->tif_decodestrip = Fax4Decode;
		tif->tif_decodetile = Fax4Decode;
		tif->tif_encoderow = Fax4Encode;
		tif->tif_encodestrip = Fax4Encode;
		tif->tif_encodetile = Fax4Encode;
		tif->tif_postencode = Fax4PostEncode;
		


		return TIFFSetField(tif, TIFFTAG_FAXMODE, FAXMODE_NORTC);
	} else
		return (0);
}
