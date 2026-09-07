static int
TIFFWriteShortArray(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir, uint32 n, uint16* v)
{
	dir->tdir_tag = tag;
	dir->tdir_type = (short) type;
	dir->tdir_count = n;
	if (n <= 2) {
		if (tif->tif_header.tiff_magic == TIFF_BIGENDIAN) {
			dir->tdir_offset = (uint32) ((long) v[0] << 16);
			if (n == 2)
				dir->tdir_offset |= v[1] & 0xffff;
		} else {
			dir->tdir_offset = v[0] & 0xffff;
			if (n == 2)
				dir->tdir_offset |= (long) v[1] << 16;
		}
		return (1);
	} else
		return (TIFFWriteData(tif, dir, (char*) v));
}
