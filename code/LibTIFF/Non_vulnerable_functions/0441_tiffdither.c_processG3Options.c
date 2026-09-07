static void
processG3Options(char* cp)
{
	if ((cp = strchr(cp, ':'))) {
		do {
			cp++;
			if (strneq(cp, "1d", 2))
				group3options &= ~GROUP3OPT_2DENCODING;
			else if (strneq(cp, "2d", 2))
				group3options |= GROUP3OPT_2DENCODING;
			else if (strneq(cp, "fill", 4))
				group3options |= GROUP3OPT_FILLBITS;
			else
				usage();
		} while ((cp = strchr(cp, ':')));
	}
}
