static void
horizontalAccumulate8abgr(uint16 *wp, int n, int stride, unsigned char *op,
	unsigned char *ToLinear8)
{
    register unsigned int  cr, cg, cb, ca, mask;
    register unsigned char  t0, t1, t2, t3;
    if (n >= stride) {
	mask = CODE_MASK;
	if (stride == 3) {
	    op[0] = 0;
	    t1 = ToLinear8[cb = (wp[2] & mask)];
	    t2 = ToLinear8[cg = (wp[1] & mask)];
	    t3 = ToLinear8[cr = (wp[0] & mask)];
	    op[1] = t1;
	    op[2] = t2;
	    op[3] = t3;
	    n -= 3;
	    while (n > 0) {
		n -= 3;
		wp += 3;
		op += 4;
		op[0] = 0;
		t1 = ToLinear8[(cb += wp[2]) & mask];
		t2 = ToLinear8[(cg += wp[1]) & mask];
		t3 = ToLinear8[(cr += wp[0]) & mask];
		op[1] = t1;
		op[2] = t2;
		op[3] = t3;
	    }
	} else if (stride == 4) {
	    t0 = ToLinear8[ca = (wp[3] & mask)];
	    t1 = ToLinear8[cb = (wp[2] & mask)];
	    t2 = ToLinear8[cg = (wp[1] & mask)];
	    t3 = ToLinear8[cr = (wp[0] & mask)];
	    op[0] = t0;
	    op[1] = t1;
	    op[2] = t2;
	    op[3] = t3;
	    n -= 4;
	    while (n > 0) {
		n -= 4;
		wp += 4;
		op += 4;
		t0 = ToLinear8[(ca += wp[3]) & mask];
		t1 = ToLinear8[(cb += wp[2]) & mask];
		t2 = ToLinear8[(cg += wp[1]) & mask];
		t3 = ToLinear8[(cr += wp[0]) & mask];
		op[0] = t0;
		op[1] = t1;
		op[2] = t2;
		op[3] = t3;
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
