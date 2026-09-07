static int
_tiffCloseProc(thandle_t fd)
{
	return (_lclose(fd));
}
