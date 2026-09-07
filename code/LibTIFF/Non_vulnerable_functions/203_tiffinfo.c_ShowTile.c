static void
ShowTile(uint32 row, uint32 col, tsample_t sample,
    unsigned char* pp, uint32 nrow, tsize_t rowsize)
{
	uint32 cc;
	printf("Tile (%lu,%lu", (unsigned long) row, (unsigned long) col);
	if (sample != (tsample_t) -1)
		printf(",%u", sample);
	printf("):\n");
	while (nrow-- > 0) {
	  for (cc = 0; cc < (uint32) rowsize; cc++) {
			printf(" %02x", *pp++);
			if (((cc+1) % 24) == 0)
				putchar('\n');
		}
		putchar('\n');
	}
}
