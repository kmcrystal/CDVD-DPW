void main()
{
	TIFF *tif=(TIFF*)0;  
	tif=XTIFFOpen("newtif.tif","w");
	if (!tif) goto failure;
	SetUpTIFFDirectory(tif);
	WriteImage(tif);
	XTIFFClose(tif);
	exit (0);
failure:
	printf("failure in maketif\n");
	if (tif) XTIFFClose(tif);
	exit (-1);
}
