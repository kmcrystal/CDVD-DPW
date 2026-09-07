static void
ShowRawBytes(unsigned char* pp, uint32 n)
{
	uint32 i;
	for (i = 0; i < n; i++) {
		printf(" %02x", *pp++);
		if (((i+1) % 24) == 0)
			printf("\n ");
	}
	putchar('\n');
}
