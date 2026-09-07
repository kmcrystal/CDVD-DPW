static toff_t
_tiffSizeProc(thandle_t fd)
{
	long pos, eof;
	pos = Fseek(0, (int) fd, SEEK_CUR);
	eof = Fseek(0, (int) fd, SEEK_END);
	Fseek(pos, (int) fd, SEEK_SET);
	return eof;
}
