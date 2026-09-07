static void
horizontalAccumulate16(uint16 *wp, int n, int stride, uint16 *op,
	uint16 *ToLinear16)
{
    register unsigned int  cr, cg, cb, ca, mask;
    if (n >= stride) {
	mask = CODE_MASK;
	if (stride == 3) {
	    op[0] = ToLinear16[cr = (wp[0] & mask)];
	    op[1] = ToLinear16[cg = (wp[1] & mask)];
	    op[2] = ToLinear16[cb = (wp[2] & mask)];
	    n -= 3;
	    while (n > 0) {
		wp += 3;
		op += 3;
		n -= 3;
		op[0] = ToLinear16[(cr += wp[0]) & mask];
		op[1] = ToLinear16[(cg += wp[1]) & mask];
		op[2] = ToLinear16[(cb += wp[2]) & mask];
	    }
	} else if (stride == 4) {
	    op[0] = ToLinear16[cr = (wp[0] & mask)];
	    op[1] = ToLinear16[cg = (wp[1] & mask)];
	    op[2] = ToLinear16[cb = (wp[2] & mask)];
	    op[3] = ToLinear16[ca = (wp[3] & mask)];
	    n -= 4;
	    while (n > 0) {
		wp += 4;
		op += 4;
		n -= 4;
		op[0] = ToLinear16[(cr += wp[0]) & mask];
		op[1] = ToLinear16[(cg += wp[1]) & mask];
		op[2] = ToLinear16[(cb += wp[2]) & mask];
		op[3] = ToLinear16[(ca += wp[3]) & mask];
	    }
	} else {
	    REPEAT(stride, *op = ToLinear16[*wp&mask]; wp++; op++)
	    n -= stride;
	    while (n > 0) {
		REPEAT(stride,
		    wp[stride] += *wp; *op = ToLinear16[*wp&mask]; wp++; op++)
		n -= stride;
	    }
	}
    }
}
