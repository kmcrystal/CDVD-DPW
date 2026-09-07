static int
TIFFFetchShortPair(TIFF* tif, TIFFDirEntry* dir)
{
	uint16 v[2];
	int ok = 0;
	switch (dir->tdir_type) {
	case TIFF_SHORT:
	case TIFF_SSHORT:
		ok = TIFFFetchShortArray(tif, dir, v);
		break;
	case TIFF_BYTE:
	case TIFF_SBYTE:
		ok  = TIFFFetchByteArray(tif, dir, v);
		break;
	}
	if (ok)
		TIFFSetField(tif, dir->tdir_tag, v[0], v[1]);
	return (ok);
}
