void
TIFFErrorExt(thandle_t fd, const char* module, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	if (_TIFFerrorHandler)
		(*_TIFFerrorHandler)(module, fmt, ap);
	if (_TIFFerrorHandlerExt)
		(*_TIFFerrorHandlerExt)(fd, module, fmt, ap);
	va_end(ap);
}
