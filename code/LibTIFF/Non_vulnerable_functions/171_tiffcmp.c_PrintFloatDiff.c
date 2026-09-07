static void
PrintFloatDiff(uint32 row, int sample, uint32 pix, double w1, double w2)
{
	if (sample < 0)
		sample = 0;
	switch (bitspersample) {
	case 32: 
		printf("Scanline %lu, pixel %lu, sample %d: %g %g\n",
		    (long) row, (long) pix, sample, w1, w2);
		if (--stopondiff == 0)
			exit(1);
		break;
	default:
		break;
	}
}
