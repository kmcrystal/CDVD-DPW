void main(int argc,char *argv[])
{
	char *fname="newtif.tif";
	int flags;
	TIFF *tif=(TIFF*)0;  
	if (argc>1) fname=argv[1];
	tif=XTIFFOpen(fname,"r");
	if (!tif) goto failure;
	
	flags = TIFFPRINT_MYMULTIDOUBLES;
	TIFFPrintDirectory(tif,stdout,flags);
	XTIFFClose(tif);
	exit (0);
failure:
	printf("failure in listtif\n");
	if (tif) XTIFFClose(tif);
	exit (-1);
}
