main()
{
	unsigned char runs[2][256];
	memset(runs[0], 0, 256*sizeof (char));
	memset(runs[1], 0, 256*sizeof (char));
	{ register int run, runlen, i;
	  runlen = 1;
	  for (run = 0x80; run != 0xff; run = (run>>1)|0x80) {
		for (i = run-1; i >= 0; i--) {
			runs[1][run|i] = runlen;
			runs[0][(~(run|i)) & 0xff] = runlen;
		}
		runlen++;
	  }
	  runs[1][0xff] = runs[0][0] = 8;
	}
	dumparray("bruns", runs[0]);
	dumparray("wruns", runs[1]);
}
