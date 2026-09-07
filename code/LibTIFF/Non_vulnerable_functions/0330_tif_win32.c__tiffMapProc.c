static int
_tiffMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	toff_t size;
	HANDLE hMapFile;
	if ((size = _tiffSizeProc(fd)) == 0xFFFFFFFF)
		return (0);
	hMapFile = CreateFileMapping(fd, NULL, PAGE_READONLY, 0, size, NULL);
	if (hMapFile == NULL)
		return (0);
	*pbase = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
	CloseHandle(hMapFile);
	if (*pbase == NULL)
		return (0);
	*psize = size;
	return(1);
}
