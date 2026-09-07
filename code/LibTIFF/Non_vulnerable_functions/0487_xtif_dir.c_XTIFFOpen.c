TIFF*
XTIFFOpen(const char* name, const char* mode)
{
	
	_XTIFFInitialize();	
	

	return TIFFOpen(name, mode);
}
