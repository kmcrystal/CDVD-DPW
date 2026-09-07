static void
PrintIntDiff(uint32 row, int sample, uint32 pix, uint32 w1, uint32 w2)
{
	if (sample < 0)
		sample = 0;
	switch (bitspersample) {
	case 1:
	case 2:
	case 4:
	    {
		int32 mask1, mask2, s;
		mask1 =  ~((-1) << bitspersample);
		s = (8 - bitspersample);
		mask2 = mask1 << s;
		for (; mask2 && pix < imagewidth;
		     mask2 >>= bitspersample, s -= bitspersample, pix++) {
			if ((w1 & mask2) ^ (w2 & mask2)) {
				printf(
			"Scanline %lu, pixel %lu, sample %d: %01x %01x\n",
	    				(unsigned long) row,
					(unsigned long) pix,
					sample,
					(unsigned int)((w1 >> s) & mask1),
					(unsigned int)((w2 >> s) & mask1));
				if (--stopondiff == 0)
					exit(1);
			}
		}
		break;
	    }
	case 8: 
		printf("Scanline %lu, pixel %lu, sample %d: %02x %02x\n",
		       (unsigned long) row, (unsigned long) pix, sample,
		       (unsigned int) w1, (unsigned int) w2);
		if (--stopondiff == 0)
			exit(1);
		break;
	case 16:
		printf("Scanline %lu, pixel %lu, sample %d: %04x %04x\n",
		    (unsigned long) row, (unsigned long) pix, sample,
		    (unsigned int) w1, (unsigned int) w2);
		if (--stopondiff == 0)
			exit(1);
		break;
	case 32:
		printf("Scanline %lu, pixel %lu, sample %d: %08x %08x\n",
		    (unsigned long) row, (unsigned long) pix, sample,
		    (unsigned int) w1, (unsigned int) w2);
		if (--stopondiff == 0)
			exit(1);
		break;
	default:
		break;
	}
}
