void
_TIFFSetDefaultCompressionState(TIFF* tif)
{
	tif->tif_decodestatus = TRUE;
	tif->tif_setupdecode = _TIFFtrue;
	tif->tif_predecode = _TIFFNoPreCode;
	tif->tif_decoderow = _TIFFNoRowDecode;
	tif->tif_decodestrip = _TIFFNoStripDecode;
	tif->tif_decodetile = _TIFFNoTileDecode;
	tif->tif_encodestatus = TRUE;
	tif->tif_setupencode = _TIFFtrue;
	tif->tif_preencode = _TIFFNoPreCode;
	tif->tif_postencode = _TIFFtrue;
	tif->tif_encoderow = _TIFFNoRowEncode;
	tif->tif_encodestrip = _TIFFNoStripEncode;
	tif->tif_encodetile = _TIFFNoTileEncode;
	tif->tif_close = _TIFFvoid;
	tif->tif_seek = _TIFFNoSeek;
	tif->tif_cleanup = _TIFFvoid;
	tif->tif_defstripsize = _TIFFDefaultStripSize;
	tif->tif_deftilesize = _TIFFDefaultTileSize;
	tif->tif_flags &= ~(TIFF_NOBITREV|TIFF_NOREADRAW);
}
