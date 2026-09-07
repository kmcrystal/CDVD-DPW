void
TIFFWarningExt(thandle_t fd, const char* module, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	if (_TIFFwarningHandler)
		(*_TIFFwarningHandler)(module, fmt, ap);
	if (_TIFFwarningHandlerExt)
		(*_TIFFwarningHandlerExt)(fd, module, fmt, ap);
	va_end(ap);
}
