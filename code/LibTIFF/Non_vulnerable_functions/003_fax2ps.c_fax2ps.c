void
fax2ps(TIFF* tif, uint16 npages, uint16* pages, char* filename)
{
    if (npages > 0) {
	uint16 pn, ptotal;
	int i;
	if (!GetPageNumber(tif))
	    fprintf(stderr, "%s: No page numbers, counting directories.\n",
		filename);
	for (i = 0; i < npages; i++) {
	    if (findPage(tif, pages[i]))
		printTIF(tif, pages[i]);
	    else
		fprintf(stderr, "%s: No page number %d\n", filename, pages[i]);
	}
    } else {
	uint16 pageNumber = 0;
	do
	    printTIF(tif, pageNumber++);
	while (TIFFReadDirectory(tif));
    }
}
