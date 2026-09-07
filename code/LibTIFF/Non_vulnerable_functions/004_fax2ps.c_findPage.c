int
findPage(TIFF* tif, uint16 pageNumber)
{
    uint16 pn = (uint16) -1;
    uint16 ptotal = (uint16) -1;
    if (GetPageNumber(tif)) {
	while (pn != (pageNumber-1) && TIFFReadDirectory(tif) && GetPageNumber(tif))
	    ;
	return (pn == (pageNumber-1));
    } else
	return (TIFFSetDirectory(tif, (tdir_t)(pageNumber-1)));
}
