static void
_TIFFprintAsciiBounded(FILE* fd, const char* cp, size_t max_chars)
{
	for (; max_chars > 0 && *cp != '\0'; cp++, max_chars--) {
		const char* tp;
		if (isprint((int)*cp)) {
			fputc(*cp, fd);
			continue;
		}
		for (tp = "\tt\bb\rr\nn\vv"; *tp; tp++)
			if (*tp++ == *cp)
				break;
		if (*tp)
			fprintf(fd, "\\%c", *tp);
		else
			fprintf(fd, "\\%03o", *cp & 0xff);
	}
}
