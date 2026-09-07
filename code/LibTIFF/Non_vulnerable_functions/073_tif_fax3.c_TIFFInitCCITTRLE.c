int
TIFFInitCCITTRLE(TIFF* tif, int scheme)
{
	(void) scheme;
	if (InitCCITTFax3(tif)) {		
		tif->tif_decoderow = Fax3DecodeRLE;
		tif->tif_decodestrip = Fax3DecodeRLE;
		tif->tif_decodetile = Fax3DecodeRLE;
		


		return TIFFSetField(tif, TIFFTAG_FAXMODE,
		    FAXMODE_NORTC|FAXMODE_NOEOL|FAXMODE_BYTEALIGN);
	} else
		return (0);
}
