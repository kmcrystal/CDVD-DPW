void
_TIFFprintAscii(FILE* fd, const char* cp)
{
	_TIFFprintAsciiBounded( fd, cp, strlen(cp));
}
