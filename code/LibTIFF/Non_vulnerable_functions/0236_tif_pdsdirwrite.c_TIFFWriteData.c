static int
TIFFWriteData(TIFF* tif, TIFFDirEntry* dir, char* cp)
{
	tsize_t cc;
	if (tif->tif_flags & TIFF_SWAB) {
		switch (dir->tdir_type) {
		case TIFF_SHORT:
		case TIFF_SSHORT:
			TIFFSwabArrayOfShort((uint16*) cp, dir->tdir_count);
			break;
		case TIFF_LONG:
		case TIFF_SLONG:
		case TIFF_FLOAT:
			TIFFSwabArrayOfLong((uint32*) cp, dir->tdir_count);
			break;
		case TIFF_RATIONAL:
		case TIFF_SRATIONAL:
			TIFFSwabArrayOfLong((uint32*) cp, 2*dir->tdir_count);
			break;
		case TIFF_DOUBLE:
			TIFFSwabArrayOfDouble((double*) cp, dir->tdir_count);
			break;
		}
	}
	dir->tdir_offset = tif->tif_dataoff;
	cc = dir->tdir_count * TIFFDataWidth(dir->tdir_type);
	if (SeekOK(tif, dir->tdir_offset) &&
	    WriteOK(tif, cp, cc)) {
		tif->tif_dataoff += (cc + 1) & ~1;
		return (1);
	}
	TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Error writing data for field \"%s\"",
	    _TIFFFieldWithTag(tif, dir->tdir_tag)->field_name);
	return (0);
}
