static int
TIFFFetchByteArray(TIFF* tif, TIFFDirEntry* dir, uint16* v)
{
	if (dir->tdir_count <= 4) {
		


		if (tif->tif_header.tiff_magic == TIFF_BIGENDIAN) {
			switch (dir->tdir_count) {
			case 4: v[3] = dir->tdir_offset & 0xff;
			case 3: v[2] = (dir->tdir_offset >> 8) & 0xff;
			case 2: v[1] = (dir->tdir_offset >> 16) & 0xff;
			case 1: v[0] = dir->tdir_offset >> 24;
			}
		} else {
			switch (dir->tdir_count) {
			case 4: v[3] = dir->tdir_offset >> 24;
			case 3: v[2] = (dir->tdir_offset >> 16) & 0xff;
			case 2: v[1] = (dir->tdir_offset >> 8) & 0xff;
			case 1: v[0] = dir->tdir_offset & 0xff;
			}
		}
		return (1);
	} else
		return (TIFFFetchData(tif, dir, (char*) v) != 0);	
}
