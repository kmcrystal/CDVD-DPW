static void
ShowRawWords(uint16* pp, uint32 n)
{
	uint32 i;
	for (i = 0; i < n; i++) {
		printf(" %04x", *pp++);
		if (((i+1) % 15) == 0)
			printf("\n ");
	}
	putchar('\n');
}
