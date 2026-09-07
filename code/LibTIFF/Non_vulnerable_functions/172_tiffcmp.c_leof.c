static void
leof(const char* name, uint32 row, int s)
{
	printf("%s: EOF at scanline %lu", name, (unsigned long)row);
	if (s >= 0)
		printf(", sample %d", s);
	printf("\n");
}
