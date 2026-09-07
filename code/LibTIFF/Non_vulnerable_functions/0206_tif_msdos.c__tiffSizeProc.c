static toff_t
_tiffSizeProc(thandle_t fd)
{
	struct stat sb;
	return (fstat((int) fd, &sb) < 0 ? 0 : sb.st_size);
}
