static toff_t
_tiffSizeProc(thandle_t fd)
{
	return (lseek((int) fd, SEEK_END, SEEK_SET));
}
