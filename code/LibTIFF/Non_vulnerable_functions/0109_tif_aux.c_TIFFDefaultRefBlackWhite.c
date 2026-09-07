static int
TIFFDefaultRefBlackWhite(TIFFDirectory* td)
{
	int i;
	if (!(td->td_refblackwhite = (float *)_TIFFmalloc(6*sizeof (float))))
		return 0;
        if (td->td_photometric == PHOTOMETRIC_YCBCR) {
		



		td->td_refblackwhite[0] = 0.0F;
		td->td_refblackwhite[1] = td->td_refblackwhite[3] =
			td->td_refblackwhite[5] = 255.0F;
		td->td_refblackwhite[2] = td->td_refblackwhite[4] = 128.0F;
	} else {
		


		for (i = 0; i < 3; i++) {
		    td->td_refblackwhite[2*i+0] = 0;
		    td->td_refblackwhite[2*i+1] =
			    (float)((1L<<td->td_bitspersample)-1L);
		}
	}
	return 1;
}
