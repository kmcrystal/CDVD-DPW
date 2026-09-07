static int
_tiffCloseProc(thandle_t fd)
{
	long r;
	r = Fclose((int) fd);
	if (r < 0) {
		errno = (int)-r;
		r = -1;
	}
	return (int)r;
}
