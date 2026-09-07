static int
TIFFWriteByteArray(TIFF* tif, TIFFDirEntry* dir, char* cp)
{
	if (dir->tdir_count > 4) {
		if (!TIFFWriteData(tif, dir, cp))
			return (0);
	} else
		_TIFFmemcpy(&dir->tdir_offset, cp, dir->tdir_count);
	return (1);
}
