static void
usage(void)
{
	char buf[BUFSIZ];
	int i;
	setbuf(stderr, buf);
	fprintf(stderr, "%s\n\n", TIFFGetVersion());
	for (i = 0; stuff[i] != NULL; i++)
		fprintf(stderr, "%s\n", stuff[i]);
	exit(-1);
}
