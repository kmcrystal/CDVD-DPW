static void JBIGOutputBie(unsigned char* buffer, size_t len, void* userData)
{
	TIFF* tif = (TIFF*)userData;
	if (isFillOrder(tif, tif->tif_dir.td_fillorder))
	{
		TIFFReverseBits(buffer, (tmsize_t)len);
	}
	JBIGCopyEncodedData(tif, buffer, len, 0);
}
