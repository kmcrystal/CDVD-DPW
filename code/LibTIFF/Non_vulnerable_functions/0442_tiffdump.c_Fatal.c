static void
Fatal(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vError(stderr, fmt, ap);
	va_end(ap);
	exit(-1);
}
