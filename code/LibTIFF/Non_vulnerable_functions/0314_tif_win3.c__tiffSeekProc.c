static toff_t
_tiffSeekProc(thandle_t fd, toff_t off, int whence)
{
	return (_llseek(fd, (off_t) off, whence));
}
