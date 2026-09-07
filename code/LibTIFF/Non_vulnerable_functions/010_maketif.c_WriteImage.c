void WriteImage(TIFF *tif)
{
	int i;
	char buffer[WIDTH];
	memset(buffer,0,sizeof(buffer));
	for (i=0;i<HEIGHT;i++)
		if (!TIFFWriteScanline(tif, buffer, i, 0))
			TIFFErrorExt(tif->tif_clientdata, "WriteImage","failure in WriteScanline\n");
}
