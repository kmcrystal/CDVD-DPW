static void
horizontalAccumulate8(uint16 *wp, int n, int stride, unsigned char *op,
	unsigned char *ToLinear8)
{
    register unsigned int  cr, cg, cb, ca, mask;
    if (n >= stride) {
	mask = CODE_MASK;
	if (stride == 3) {
	    op[0] = ToLinear8[cr = (wp[0] & mask)];
	    op[1] = ToLinear8[cg = (wp[1] & mask)];
	    op[2] = ToLinear8[cb = (wp[2] & mask)];
	    n -= 3;
	    while (n > 0) {
		n -= 3;
		wp += 3;
		op += 3;
		op[0] = ToLinear8[(cr += wp[0]) & mask];
		op[1] = ToLinear8[(cg += wp[1]) & mask];
		op[2] = ToLinear8[(cb += wp[2]) & mask];
	    }
	} else if (stride == 4) {
	    op[0] = ToLinear8[cr = (wp[0] & mask)];
	    op[1] = ToLinear8[cg = (wp[1] & mask)];
	    op[2] = ToLinear8[cb = (wp[2] & mask)];
	    op[3] = ToLinear8[ca = (wp[3] & mask)];
	    n -= 4;
	    while (n > 0) {
		n -= 4;
		wp += 4;
		op += 4;
		op[0] = ToLinear8[(cr += wp[0]) & mask];
		op[1] = ToLinear8[(cg += wp[1]) & mask];
		op[2] = ToLinear8[(cb += wp[2]) & mask];
		op[3] = ToLinear8[(ca += wp[3]) & mask];
	    }
	} else {
	    REPEAT(stride, *op = ToLinear8[*wp&mask]; wp++; op++)
	    n -= stride;
	    while (n > 0) {
		REPEAT(stride,
		    wp[stride] += *wp; *op = ToLinear8[*wp&mask]; wp++; op++)
		n -= stride;
	    }
	}
    }
}
