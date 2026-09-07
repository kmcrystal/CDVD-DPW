static int
_tiffCloseProc(thandle_t fd)
{
	return (FSClose((short) fd));
}
