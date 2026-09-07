static int
NotConfigured(TIFF* tif, int scheme)
{
	(void) scheme;
	tif->tif_fixuptags = _notConfigured;
	tif->tif_decodestatus = FALSE;
	tif->tif_setupdecode = _notConfigured;
	tif->tif_encodestatus = FALSE;
	tif->tif_setupencode = _notConfigured;
	return (1);
}
