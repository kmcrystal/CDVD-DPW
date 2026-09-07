static tsize_t
_tiffReadProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	DWORD dwSizeRead;
	if (!ReadFile(fd, buf, size, &dwSizeRead, NULL))
		return(0);
	return ((tsize_t) dwSizeRead);
}
