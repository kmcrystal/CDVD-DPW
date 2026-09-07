int
main(int argc, char **argv)
{
	uint32		rowsperstrip;
	uint16		compression;
	uint16		spp, bps, photometric, sampleformat, planarconfig;
        (void) argc;
        (void) argv;
	



	rowsperstrip = 1;
	compression = COMPRESSION_NONE;
	spp = 1;
	bps = 8;
        photometric = PHOTOMETRIC_MINISBLACK;
	sampleformat = SAMPLEFORMAT_UINT;
	planarconfig = PLANARCONFIG_CONTIG;
	if (create_image_striped(filename, XSIZE * YSIZE, 1, rowsperstrip,
				  compression, spp, bps, photometric,
				  sampleformat, planarconfig,
				  (const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't create TIFF file %s.\n", filename);
		goto failure;
	}
	if (read_image_striped(filename, XSIZE * YSIZE, 1, rowsperstrip,
				compression, spp, bps, photometric,
				sampleformat, planarconfig,
				(const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't read TIFF file %s.\n", filename);
		goto failure;
	}
	unlink(filename);
	if (create_image_striped(filename, 1, XSIZE * YSIZE, rowsperstrip,
				  compression, spp, bps, photometric,
				  sampleformat, planarconfig,
				  (const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't create TIFF file %s.\n", filename);
		goto failure;
	}
	if (read_image_striped(filename, 1, XSIZE * YSIZE, rowsperstrip,
				compression, spp, bps, photometric,
				sampleformat, planarconfig,
				(const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't read TIFF file %s.\n", filename);
		goto failure;
	}
	unlink(filename);
	


	rowsperstrip = 1;
	spp = 1;
	bps = 8;
        photometric = PHOTOMETRIC_MINISBLACK;
	sampleformat = SAMPLEFORMAT_UINT;
	planarconfig = PLANARCONFIG_CONTIG;
	if (create_image_striped(filename, XSIZE, YSIZE, rowsperstrip,
				  compression, spp, bps, photometric,
				  sampleformat, planarconfig,
				  (const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't create TIFF file %s.\n", filename);
		goto failure;
	}
	if (read_image_striped(filename, XSIZE, YSIZE, rowsperstrip,
				compression, spp, bps, photometric,
				sampleformat, planarconfig,
				(const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't read TIFF file %s.\n", filename);
		goto failure;
	}
	unlink(filename);
	rowsperstrip = YSIZE;
	if (create_image_striped(filename, XSIZE, YSIZE, rowsperstrip,
				  compression, spp, bps, photometric,
				  sampleformat, planarconfig,
				  (const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't create TIFF file %s.\n", filename);
		goto failure;
	}
	if (read_image_striped(filename, XSIZE, YSIZE, rowsperstrip,
				compression, spp, bps, photometric,
				sampleformat, planarconfig,
				(const tdata_t) byte_array1, byte_array1_size) < 0) {
		fprintf (stderr, "Can't read TIFF file %s.\n", filename);
		goto failure;
	}
	unlink(filename);
	return 0;
failure:
	unlink(filename);
	return 1;
}
