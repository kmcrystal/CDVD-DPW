static int
CheckDirCount(TIFF* tif, TIFFDirEntry* dir, uint32 count)
{
	if (count != dir->tdir_count) {
		TIFFWarning(tif->tif_name,
	"incorrect count for field \"%s\" (%lu, expecting %lu); tag ignored",
		    _TIFFFieldWithTag(tif, dir->tdir_tag)->field_name,
		    dir->tdir_count, count);
		return (0);
	}
	return (1);
}
