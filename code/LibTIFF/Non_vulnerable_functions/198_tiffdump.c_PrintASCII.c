static void
PrintASCII(FILE* fd, uint32 cc, const unsigned char* cp)
{
	for (; cc > 0; cc--, cp++) {
		const char* tp;
		if (isprint(*cp)) {
			fputc(*cp, fd);
			continue;
		}
		for (tp = "\tt\bb\rr\nn\vv"; *tp; tp++)
			if (*tp++ == *cp)
				break;
		if (*tp)
			fprintf(fd, "\\%c", *tp);
		else if (*cp)
			fprintf(fd, "\\%03o", *cp);
		else
			fprintf(fd, "\\0");
	}
}
