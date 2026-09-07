static tsize_t
_tiffWriteProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	long r;
	r = Fwrite((int) fd, size, buf);
	if (r < 0) {
		errno = (int)-r;
		r = -1;
	}
	return r;
}
