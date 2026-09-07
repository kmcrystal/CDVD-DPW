static void
_tiffUnmapProc(thandle_t fd, tdata_t base, toff_t size)
{
	UnmapViewOfFile(base);
}
