static void
horizontalAccumulate12(uint16 *wp, int n, int stride, int16 *op,
	float *ToLinearF)
{
    register unsigned int  cr, cg, cb, ca, mask;
    register float  t0, t1, t2, t3;
    if (n >= stride) {
	mask = CODE_MASK;
	if (stride == 3) {
	    t0 = ToLinearF[cr = (wp[0] & mask)] * SCALE12;
	    t1 = ToLinearF[cg = (wp[1] & mask)] * SCALE12;
	    t2 = ToLinearF[cb = (wp[2] & mask)] * SCALE12;
	    op[0] = CLAMP12(t0);
	    op[1] = CLAMP12(t1);
	    op[2] = CLAMP12(t2);
	    n -= 3;
	    while (n > 0) {
		wp += 3;
		op += 3;
		n -= 3;
		t0 = ToLinearF[(cr += wp[0]) & mask] * SCALE12;
		t1 = ToLinearF[(cg += wp[1]) & mask] * SCALE12;
		t2 = ToLinearF[(cb += wp[2]) & mask] * SCALE12;
		op[0] = CLAMP12(t0);
		op[1] = CLAMP12(t1);
		op[2] = CLAMP12(t2);
	    }
	} else if (stride == 4) {
	    t0 = ToLinearF[cr = (wp[0] & mask)] * SCALE12;
	    t1 = ToLinearF[cg = (wp[1] & mask)] * SCALE12;
	    t2 = ToLinearF[cb = (wp[2] & mask)] * SCALE12;
	    t3 = ToLinearF[ca = (wp[3] & mask)] * SCALE12;
	    op[0] = CLAMP12(t0);
	    op[1] = CLAMP12(t1);
	    op[2] = CLAMP12(t2);
	    op[3] = CLAMP12(t3);
	    n -= 4;
	    while (n > 0) {
		wp += 4;
		op += 4;
		n -= 4;
		t0 = ToLinearF[(cr += wp[0]) & mask] * SCALE12;
		t1 = ToLinearF[(cg += wp[1]) & mask] * SCALE12;
		t2 = ToLinearF[(cb += wp[2]) & mask] * SCALE12;
		t3 = ToLinearF[(ca += wp[3]) & mask] * SCALE12;
		op[0] = CLAMP12(t0);
		op[1] = CLAMP12(t1);
		op[2] = CLAMP12(t2);
		op[3] = CLAMP12(t3);
	    }
	} else {
	    REPEAT(stride, t0 = ToLinearF[*wp&mask] * SCALE12;
                           *op = CLAMP12(t0); wp++; op++)
	    n -= stride;
	    while (n > 0) {
		REPEAT(stride,
		    wp[stride] += *wp; t0 = ToLinearF[wp[stride]&mask]*SCALE12;
		    *op = CLAMP12(t0);  wp++; op++)
		n -= stride;
	    }
	}
    }
}
