void
_TIFFprintAsciiTag(FILE* fd, const char* name, const char* value)
{
	fprintf(fd, "  %s: \"", name);
	_TIFFprintAscii(fd, value);
	fprintf(fd, "\"\n");
}
