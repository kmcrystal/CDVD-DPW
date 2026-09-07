static void
vError(FILE* fd, const char* fmt, va_list ap)
{
	fprintf(fd, "%s: ", curfile);
	vfprintf(fd, fmt, ap);
	fprintf(fd, ".\n");
}
