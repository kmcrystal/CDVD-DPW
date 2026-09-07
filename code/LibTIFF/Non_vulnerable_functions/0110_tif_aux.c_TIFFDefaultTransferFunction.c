static int
TIFFDefaultTransferFunction(TIFFDirectory* td)
{
	uint16 **tf = td->td_transferfunction;
	tmsize_t i, n, nbytes;
	tf[0] = tf[1] = tf[2] = 0;
	if (td->td_bitspersample >= sizeof(tmsize_t) * 8 - 2)
		return 0;
	n = ((tmsize_t)1)<<td->td_bitspersample;
	nbytes = n * sizeof (uint16);
	if (!(tf[0] = (uint16 *)_TIFFmalloc(nbytes)))
		return 0;
	tf[0][0] = 0;
	for (i = 1; i < n; i++) {
		double t = (double)i/((double) n-1.);
		tf[0][i] = (uint16)floor(65535.*pow(t, 2.2) + .5);
	}
	if (td->td_samplesperpixel - td->td_extrasamples > 1) {
		if (!(tf[1] = (uint16 *)_TIFFmalloc(nbytes)))
			goto bad;
		_TIFFmemcpy(tf[1], tf[0], nbytes);
		if (!(tf[2] = (uint16 *)_TIFFmalloc(nbytes)))
			goto bad;
		_TIFFmemcpy(tf[2], tf[0], nbytes);
	}
	return 1;
bad:
	if (tf[0])
		_TIFFfree(tf[0]);
	if (tf[1])
		_TIFFfree(tf[1]);
	if (tf[2])
		_TIFFfree(tf[2]);
	tf[0] = tf[1] = tf[2] = 0;
	return 0;
}
