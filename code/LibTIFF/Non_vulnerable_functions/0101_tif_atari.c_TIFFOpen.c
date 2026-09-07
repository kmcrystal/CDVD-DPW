TIFF*
TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	int m;
	long fd;
	m = _TIFFgetMode(mode, module);
	if (m == -1)
		return ((TIFF*)0);
	if (m & O_TRUNC) {
		fd = Fcreate(name, 0);
	} else {
		fd = Fopen(name, m & O_ACCMODE);
		if (fd == AEFILNF && m & O_CREAT)
			fd = Fcreate(name, 0);
	}
	if (fd < 0)
		errno = (int)fd;
	if (fd < 0) {
		TIFFErrorExt(0, module, "%s: Cannot open", name);
		return ((TIFF*)0);
	}
	return (TIFFFdOpen(fd, name, mode));
}
