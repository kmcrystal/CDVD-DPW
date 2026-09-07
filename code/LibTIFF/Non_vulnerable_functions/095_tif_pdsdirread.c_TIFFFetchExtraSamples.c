static int
TIFFFetchExtraSamples(TIFF* tif, TIFFDirEntry* dir)
{
	uint16 buf[10];
	uint16* v = buf;
	int status;
	if (dir->tdir_count > NITEMS(buf))
		v = (uint16*) _TIFFmalloc(dir->tdir_count * sizeof (uint16));
	if (dir->tdir_type == TIFF_BYTE)
		status = TIFFFetchByteArray(tif, dir, v);
	else
		status = TIFFFetchShortArray(tif, dir, v);
	if (status)
		status = TIFFSetField(tif, dir->tdir_tag, dir->tdir_count, v);
	if (v != buf)
		_TIFFfree((char*) v);
	return (status);
}
