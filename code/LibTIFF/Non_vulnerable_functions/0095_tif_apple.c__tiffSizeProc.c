static toff_t
_tiffSizeProc(thandle_t fd)
{
	long size;
	if (GetEOF((short) fd, &size) != noErr) {
		TIFFErrorExt(fd, "_tiffSizeProc", "%s: Cannot get file size");
		return (-1L);
	}
	return ((toff_t) size);
}
