static int
TIFFFetchRefBlackWhite(TIFF* tif, TIFFDirEntry* dir)
{
	static char mesg[] = "for \"ReferenceBlackWhite\" array";
	char* cp;
	int ok;
	if (dir->tdir_type == TIFF_RATIONAL)
		return (1);
	


	cp = CheckMalloc(tif, dir->tdir_count * sizeof (uint32), mesg);
	if (ok = (cp && TIFFFetchLongArray(tif, dir, (uint32*) cp))) {
		float* fp = (float*)
		    CheckMalloc(tif, dir->tdir_count * sizeof (float), mesg);
		if (ok = (fp != NULL)) {
			uint32 i;
			for (i = 0; i < dir->tdir_count; i++)
				fp[i] = (float)((uint32*) cp)[i];
			ok = TIFFSetField(tif, dir->tdir_tag, fp);
			_TIFFfree((char*) fp);
		}
	}
	if (cp)
		_TIFFfree(cp);
	return (ok);
}
