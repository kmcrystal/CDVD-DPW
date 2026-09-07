static int
initImage(void)
{
        uint32 w, h;
        if (order)
                TIFFSetField(tif, TIFFTAG_FILLORDER, order);
        if (photo != (uint16) -1)
                TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photo);
        if (!TIFFRGBAImageBegin(&img, tif, stoponerr, title)) {
                TIFFError(filelist[fileindex], "%s", title);
                TIFFClose(tif);
                tif = NULL;
                return -1;
        }
        


        h = img.height;
        w = img.width;
        if (h > ymax) {
                w = (int)(w * ((float)ymax / h));
                h = ymax;
        }
        if (w > xmax) {
                h = (int)(h * ((float)xmax / w));
                w = xmax;
        }
	if (w != width || h != height) {
		uint32 rastersize =
			_TIFFMultiply32(tif, img.width, img.height, "allocating raster buffer");
		if (raster != NULL)
			_TIFFfree(raster), raster = NULL;
		raster = (uint32*) _TIFFCheckMalloc(tif, rastersize, sizeof (uint32),
						    "allocating raster buffer");
		if (raster == NULL) {
			width = height = 0;
			TIFFError(filelist[fileindex], "No space for raster buffer");
			cleanup_and_exit();
		}
		width = w;
		height = h;
	}
	TIFFRGBAImageGet(&img, raster, img.width, img.height);
#if HOST_BIGENDIAN
	TIFFSwabArrayOfLong(raster,img.width*img.height);
#endif
	return 0;
}
