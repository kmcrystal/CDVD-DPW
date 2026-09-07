static int
TIFFFetchStripThing(TIFF* tif, TIFFDirEntry* dir, long nstrips, uint32** lpp)
{
	register uint32* lp;
	int status;
	if (!CheckDirCount(tif, dir, (uint32) nstrips))
		return (0);
	


	if (*lpp == NULL &&
	    (*lpp = (uint32 *)CheckMalloc(tif,
	      nstrips * sizeof (uint32), "for strip array")) == NULL)
		return (0);
	lp = *lpp;
	if (dir->tdir_type == (int)TIFF_SHORT) {
		


		uint16* dp = (uint16*) CheckMalloc(tif,
		    dir->tdir_count* sizeof (uint16), "to fetch strip tag");
		if (dp == NULL)
			return (0);
		if (status = TIFFFetchShortArray(tif, dir, dp)) {
			register uint16* wp = dp;
			while (nstrips-- > 0)
				*lp++ = *wp++;
		}
		_TIFFfree((char*) dp);
	} else
		status = TIFFFetchLongArray(tif, dir, lp);
	return (status);
}
