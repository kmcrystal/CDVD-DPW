TIFF*
TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	int m, fd;
	m = _TIFFgetMode(mode, module);
	if (m == -1)
	{
		return ((TIFF*) 0);
	}
	fd = open(name, 0, m);
	if (fd < 0)
	{
		TIFFErrorExt(0, module, "%s: Cannot open", name);
		return ((TIFF *)0);
	}
	return (TIFFFdOpen(fd, name, mode));
}
