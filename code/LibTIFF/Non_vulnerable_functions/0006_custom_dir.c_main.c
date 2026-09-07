int
main()
{
	TIFF		*tif;
	unsigned char	buf[SPP] = { 0, 127, 255 };
	uint64          dir_offset = 0, dir_offset2 = 0;
	uint64          read_dir_offset = 0, read_dir_offset2 = 0;
	uint64          *dir_offset2_ptr = NULL;
	char           *ascii_value;
	uint16          count16 = 0;
	
	tif = TIFFOpen(filename, "w+");
	if (!tif) {
		fprintf (stderr, "Can't create test TIFF file %s.\n", filename);
		return 1;
	}
	if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width)) {
		fprintf (stderr, "Can't set ImageWidth tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length)) {
		fprintf (stderr, "Can't set ImageLength tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bps)) {
		fprintf (stderr, "Can't set BitsPerSample tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, SPP)) {
		fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip)) {
		fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig)) {
		fprintf (stderr, "Can't set PlanarConfiguration tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric)) {
		fprintf (stderr, "Can't set PhotometricInterpretation tag.\n");
		goto failure;
	}
	
	if (TIFFWriteScanline(tif, buf, 0, 0) == -1) {
		fprintf (stderr, "Can't write image data.\n");
		goto failure;
	}
        if (!TIFFWriteDirectory( tif )) {
		fprintf (stderr, "TIFFWriteDirectory() failed.\n");
		goto failure;
	}
	


	if (TIFFCreateEXIFDirectory(tif) != 0) {
		fprintf (stderr, "TIFFCreateEXIFDirectory() failed.\n" );
		goto failure;
	}
	if (!TIFFSetField( tif, EXIFTAG_SPECTRALSENSITIVITY, "EXIF Spectral Sensitivity")) {
		fprintf (stderr, "Can't write SPECTRALSENSITIVITY\n" );
		goto failure;
	}		
        if (!TIFFWriteCustomDirectory( tif, &dir_offset )) {
		fprintf (stderr, "TIFFWriteCustomDirectory() with EXIF failed.\n");
		goto failure;
	}
	



	TIFFFreeDirectory( tif );
	if (TIFFCreateCustomDirectory(tif, &customFieldArray) != 0) {
		fprintf (stderr, "TIFFCreateEXIFDirectory() failed.\n" );
		goto failure;
	}
	if (!TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, "*Custom1")) { 
		fprintf (stderr, "Can't write pseudo-IMAGEWIDTH.\n" );
		goto failure;
	}		
	if (!TIFFSetField( tif, TIFFTAG_DOTRANGE, "*Custom2")) { 
		fprintf (stderr, "Can't write pseudo-DOTWIDTH.\n" );
		goto failure;
	}		
        if (!TIFFWriteCustomDirectory( tif, &dir_offset2 )) {
		fprintf (stderr, "TIFFWriteCustomDirectory() with EXIF failed.\n");
		goto failure;
	}
	


	TIFFSetDirectory(tif, 0);
	TIFFSetField(tif, TIFFTAG_EXIFIFD, dir_offset );
	TIFFSetField(tif, TIFFTAG_SUBIFD, 1, &dir_offset2 );
	TIFFClose(tif);
	
	tif = TIFFOpen(filename, "r");
	TIFFGetField(tif, TIFFTAG_EXIFIFD, &read_dir_offset );
	if( read_dir_offset != dir_offset ) {
		fprintf (stderr, "Did not get expected EXIFIFD.\n" );
		goto failure;
	}
	TIFFGetField(tif, TIFFTAG_SUBIFD, &count16, &dir_offset2_ptr );
	read_dir_offset2 = dir_offset2_ptr[0];
	if( read_dir_offset2 != dir_offset2 || count16 != 1) {
		fprintf (stderr, "Did not get expected SUBIFD.\n" );
		goto failure;
	}
	if( !TIFFReadEXIFDirectory(tif, read_dir_offset) ) {
		fprintf (stderr, "TIFFReadEXIFDirectory() failed.\n" );
		goto failure;
	}
	if (!TIFFGetField( tif, EXIFTAG_SPECTRALSENSITIVITY, &ascii_value) ) {
		fprintf (stderr, "reading SPECTRALSENSITIVITY failed.\n" );
		goto failure;
	}
	if( strcmp(ascii_value,"EXIF Spectral Sensitivity") != 0) {
		fprintf (stderr, "got wrong SPECTRALSENSITIVITY value.\n" );
		goto failure;
	}
	
	if( !TIFFReadCustomDirectory(tif, read_dir_offset2, &customFieldArray) ) {
		fprintf (stderr, "TIFFReadCustomDirectory() failed.\n" );
		goto failure;
	}
	if (!TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &ascii_value) ) {
		fprintf (stderr, "reading pseudo-IMAGEWIDTH failed.\n" );
		goto failure;
	}
	if( strcmp(ascii_value,"*Custom1") != 0) {
		fprintf (stderr, "got wrong pseudo-IMAGEWIDTH value.\n" );
		goto failure;
	}
	if (!TIFFGetField( tif, TIFFTAG_DOTRANGE, &ascii_value) ) {
		fprintf (stderr, "reading pseudo-DOTRANGE failed.\n" );
		goto failure;
	}
	if( strcmp(ascii_value,"*Custom2") != 0) {
		fprintf (stderr, "got wrong pseudo-DOTRANGE value.\n" );
		goto failure;
	}
	TIFFClose(tif);
	
	unlink(filename);
	return 0;
failure:
	



	TIFFClose(tif);
	return 1;
}
