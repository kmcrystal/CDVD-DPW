TIFF*
XTIFFFdOpen(int fd, const char* name, const char* mode)
{
	
	_XTIFFInitialize();	
	

	return TIFFFdOpen(fd, name, mode);
}
