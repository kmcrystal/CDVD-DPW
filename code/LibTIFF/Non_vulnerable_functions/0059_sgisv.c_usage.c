static void
usage(void)
{
	char buf[BUFSIZ];
	int i;
	setbuf(stderr, buf);
	for (i = 0; stuff[i] != NULL; i++)
		fprintf(stderr, "%s\n", stuff[i]);
	exit(-1);
}
