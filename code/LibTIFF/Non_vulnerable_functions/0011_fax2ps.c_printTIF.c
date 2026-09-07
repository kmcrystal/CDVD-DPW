void
printTIF(TIFF* tif, uint16 pageNumber)
{
    uint32 w, h;
    uint16 unit, compression;
    float xres, yres, scale = 1.0;
    tstrip_t s, ns;
    time_t creation_time;
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    if (!TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression)
	|| compression < COMPRESSION_CCITTRLE
	|| compression > COMPRESSION_CCITT_T6)
	return;
    if (!TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres) || !xres) {
	TIFFWarning(TIFFFileName(tif),
	    "No x-resolution, assuming %g dpi", defxres);
	xres = defxres;
    }
    if (!TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres) || !yres) {
	TIFFWarning(TIFFFileName(tif),
	    "No y-resolution, assuming %g lpi", defyres);
	yres = defyres;					
    }
    if (TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &unit) &&
      unit == RESUNIT_CENTIMETER) {
	xres *= 2.54F;
	yres *= 2.54F;
    }
    if (pageWidth == 0)
	pageWidth = w / xres;
    if (pageHeight == 0)
	pageHeight = h / yres;
    printf("%%!PS-Adobe-3.0\n");
    printf("%%%%Creator: fax2ps\n");
#ifdef notdef
    printf("%%%%Title: %s\n", file);
#endif
    creation_time = time(0);
    printf("%%%%CreationDate: %s", ctime(&creation_time));
    printf("%%%%Origin: 0 0\n");
    printf("%%%%BoundingBox: 0 0 %u %u\n",
	(int)(pageWidth * points), (int)(pageHeight * points));	
    printf("%%%%Pages: (atend)\n");
    printf("%%%%EndComments\n");
    printf("%%%%BeginProlog\n");
    emitFont(stdout);
    printf("/d{bind def}def\n"); 
    printf("/m{0 exch moveto}d\n");
    printf("/s{show}d\n");
    printf("/p{showpage}d \n");	
    printf("%%%%EndProlog\n");
    printf("%%%%Page: \"%u\" %u\n", pageNumber, pageNumber);
    printf("/$pageTop save def gsave\n");
    if (scaleToPage)
        scale = pageHeight / (h/yres) < pageWidth / (w/xres) ?
            pageHeight / (h/yres) : pageWidth / (w/xres);
    printf("%g %g translate\n",
           points * (pageWidth - scale*w/xres) * half,
           points * (scale*h/yres + (pageHeight - scale*h/yres) * half));
    printf("%g %g scale\n", points/xres*scale, -points/yres*scale);
    printf("0 setgray\n");
    TIFFSetField(tif, TIFFTAG_FAXFILLFUNC, printruns);
    ns = TIFFNumberOfStrips(tif);
    row = 0;
    for (s = 0; s < ns; s++)
	(void) TIFFReadEncodedStrip(tif, s, (tdata_t) NULL, (tsize_t) -1);
    printf("p\n");
    printf("grestore $pageTop restore\n");
    totalPages++;
}
