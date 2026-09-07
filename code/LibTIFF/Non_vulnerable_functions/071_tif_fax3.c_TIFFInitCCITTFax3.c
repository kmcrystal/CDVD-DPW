int
TIFFInitCCITTFax3(TIFF* tif, int scheme)
{
	(void) scheme;
	if (InitCCITTFax3(tif)) {
		_TIFFMergeFieldInfo(tif, fax3FieldInfo, N(fax3FieldInfo));
		


		return TIFFSetField(tif, TIFFTAG_FAXMODE, FAXMODE_CLASSF);
	} else
		return (0);
}
