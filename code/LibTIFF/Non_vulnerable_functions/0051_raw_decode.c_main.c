int
main(int argc, char **argv)
{
	TIFF		*tif;
	static const char *srcfilerel = "images/quad-tile.jpg.tiff";
	char *srcdir = NULL;
	char srcfile[1024];
	unsigned short h, v;
	int status;
	unsigned char *buffer;
	uint32 *rgba_buffer;
	tsize_t sz, szout;
	unsigned int pixel_status = 0;
        (void) argc;
        (void) argv;
	if ((srcdir = getenv("srcdir")) == NULL) {
		srcdir = ".";
	}
	if ((strlen(srcdir) + 1 + strlen(srcfilerel)) >= sizeof(srcfile)) {
		fprintf( stderr, "srcdir too long %s\n", srcdir);
		exit( 1 );
	}
	strcpy(srcfile,srcdir);
	strcat(srcfile,"/");
	strcat(srcfile,srcfilerel);
	tif = TIFFOpen(srcfile,"r");
	if ( tif == NULL ) {
		fprintf( stderr, "Could not open %s\n", srcfile);
		exit( 1 );
	}
	status = TIFFGetField(tif,TIFFTAG_YCBCRSUBSAMPLING, &h, &v);
	if ( status == 0 || h != 2 || v != 2) {
		fprintf( stderr, "Could not retrieve subsampling tag.\n" );
		exit(1);
	}
	


	sz = TIFFTileSize(tif);
	if( sz != 24576) {
		fprintf(stderr, "tiles are %d bytes\n", (int)sz);
		exit(1);
	}
	buffer = (unsigned char *) malloc(sz);
	


	szout = TIFFReadEncodedTile(tif,9,buffer,sz);
	if (szout != sz) {
		fprintf( stderr, 
			 "Did not get expected result code from TIFFReadEncodedTile()(%d instead of %d)\n", 
			 (int) szout, (int) sz );
		return 1;
	}
	if( check_cluster( 0, buffer, cluster_0 )
	    || check_cluster( 64, buffer, cluster_64 )
	    || check_cluster( 128, buffer, cluster_128 ) ) {
		exit(1);
	}
	free(buffer);
	


	TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
	sz = TIFFTileSize(tif);
	if( sz != 128*128*3) {
		fprintf(stderr, "tiles are %d bytes\n", (int)sz);
		exit(1);
	}
	buffer = (unsigned char *) malloc(sz);
	szout = TIFFReadEncodedTile(tif,9,buffer,sz);
	if (szout != sz) {
		fprintf( stderr, 
			 "Did not get expected result code from TIFFReadEncodedTile()(%d instead of %d)\n", 
			 (int) szout, (int) sz );
		return 1;
	}
	







	pixel_status |= check_rgb_pixel( 0, 15, 18, 0, 0, 18, 41, buffer );
	pixel_status |= check_rgb_pixel( 64, 0, 0, 0, 0, 0, 2, buffer );
	pixel_status |= check_rgb_pixel( 512, 5, 6, 34, 36, 182, 196, buffer );
	free( buffer );
	TIFFClose(tif);
	


	tif = TIFFOpen(srcfile,"r");
	sz = 128 * 128 * sizeof(uint32);
	rgba_buffer = (uint32 *) malloc(sz);
	if (!TIFFReadRGBATile( tif, 1*128, 2*128, rgba_buffer )) {
		fprintf( stderr, "TIFFReadRGBATile() returned failure code.\n" );
		return 1;
	}
	





	pixel_status |= check_rgba_pixel( 0, 15, 18, 0, 0, 18, 41, 255, 255,
					  rgba_buffer );
	pixel_status |= check_rgba_pixel( 64, 0, 0, 0, 0, 0, 2, 255, 255,
					  rgba_buffer );
	pixel_status |= check_rgba_pixel( 512, 5, 6, 34, 36, 182, 196, 255, 255,
					  rgba_buffer );
	free( rgba_buffer );
	TIFFClose(tif);
	if (pixel_status) {
		exit(1);
	}
	exit( 0 );
}
