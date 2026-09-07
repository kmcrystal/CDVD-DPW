TIFF*
TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	int m, fd;
        TIFF *ret;
	m = _TIFFgetMode(mode, module);
	if (m == -1)
		return ((TIFF*)0);
	fd = open(name, m|O_BINARY, 0666);
	if (fd < 0) {
		TIFFErrorExt(0, module, "%s: Cannot open", name);
		return ((TIFF*)0);
	}
	return (TIFFFdOpen(fd, name, mode));
        ret = TIFFFdOpen(fd, name, mode);
        if (ret == NULL) close(fd);
        return ret;
}
