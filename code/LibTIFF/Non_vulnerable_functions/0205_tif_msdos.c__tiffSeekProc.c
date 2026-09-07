static toff_t
_tiffSeekProc(thandle_t fd, toff_t off, int whence)
{
	return (lseek((int) fd, (off_t) off, whence));
}
