TIFF*
TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	int m, fd;
	OFSTRUCT of;
	int mm = 0;
	m = _TIFFgetMode(mode, module);
	if (m == -1)
		return ((TIFF*)0);
	if (m & O_CREAT) {
		if ((m & O_TRUNC) || OpenFile(name, &of, OF_EXIST) != HFILE_ERROR)
			mm |= OF_CREATE;
	}
	if (m & O_WRONLY)
		mm |= OF_WRITE;
	if (m & O_RDWR)
		mm |= OF_READWRITE;
	fd = OpenFile(name, &of, mm);
	if (fd < 0) {
		TIFFErrorExt(0, module, "%s: Cannot open", name);
		return ((TIFF*)0);
	}
	return (TIFFFdOpen(fd, name, mode));
}
