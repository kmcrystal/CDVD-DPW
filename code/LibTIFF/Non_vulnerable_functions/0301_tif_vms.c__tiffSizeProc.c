static toff_t
_tiffSizeProc(thandle_t fd)
{
	_TIFF_stat_s sb;
	return (toff_t) (_TIFF_fstat_f((int) fd, &sb) < 0 ? 0 : sb.st_size);
}
