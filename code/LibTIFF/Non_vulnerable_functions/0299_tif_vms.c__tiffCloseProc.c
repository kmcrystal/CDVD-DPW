static int
_tiffCloseProc(thandle_t fd)
{
	return (close((int) fd));
}
