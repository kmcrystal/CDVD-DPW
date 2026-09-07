static tsize_t
_tiffReadProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	long r;
	r = Fread((int) fd, size, buf);
	if (r < 0) {
		errno = (int)-r;
		r = -1;
	}
	return r;
}
