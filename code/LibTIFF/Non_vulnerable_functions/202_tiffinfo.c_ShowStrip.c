static void
ShowStrip(tstrip_t strip, unsigned char* pp, uint32 nrow, tsize_t scanline)
{
	register tsize_t cc;
	printf("Strip %lu:\n", (unsigned long) strip);
	while (nrow-- > 0) {
		for (cc = 0; cc < scanline; cc++) {
			printf(" %02x", *pp++);
			if (((cc+1) % 24) == 0)
				putchar('\n');
		}
		putchar('\n');
	}
}
