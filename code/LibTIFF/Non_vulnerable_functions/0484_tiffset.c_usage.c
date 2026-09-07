static void
usage(void)
{
	int i;
	for (i = 0; usageMsg[i]; i++)
		fprintf(stderr, "%s\n", usageMsg[i]);
	exit(-1);
}
