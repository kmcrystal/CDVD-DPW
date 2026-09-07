static toff_t
_tiffSeekProc(thandle_t fd, toff_t off, int whence)
{
	return ((toff_t) _TIFF_lseek_f((int) fd, (_TIFF_off_t) off, whence));
}
