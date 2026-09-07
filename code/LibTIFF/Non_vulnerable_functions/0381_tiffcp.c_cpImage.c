static int
cpImage(TIFF* in, TIFF* out, readFunc fin, writeFunc fout,
	uint32 imagelength, uint32 imagewidth, tsample_t spp)
{
	int status = 0;
	tdata_t buf = NULL;
	tsize_t scanlinesize = TIFFRasterScanlineSize(in);
	tsize_t bytes = scanlinesize * (tsize_t)imagelength;
	


	if (scanlinesize
	    && imagelength
	    && bytes / (tsize_t)imagelength == scanlinesize) {
		buf = _TIFFmalloc(bytes);
		if (buf) {
			if ((*fin)(in, (uint8*)buf, imagelength,
			    imagewidth, spp)) {
				status = (*fout)(out, (uint8*)buf,
				    imagelength, imagewidth, spp);
			}
			_TIFFfree(buf);
		} else {
			TIFFError(TIFFFileName(in),
			    "Error, can't allocate space for image buffer");
		}
	} else {
		TIFFError(TIFFFileName(in), "Error, no space for image buffer");
	}
	return status;
}
