static toff_t
_tiffSizeProc(thandle_t fd)
{
	return ((toff_t)GetFileSize(fd, NULL));
}
