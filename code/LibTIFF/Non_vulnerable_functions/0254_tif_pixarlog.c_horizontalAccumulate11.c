static void
horizontalAccumulate11(uint16 *wp, int n, int stride, uint16 *op)
{
    register unsigned int cr, cg, cb, ca, mask;
    if (n >= stride) {
	mask = CODE_MASK;
	if (stride == 3) {
	    op[0] = wp[0];  op[1] = wp[1];  op[2] = wp[2];
            cr = wp[0];  cg = wp[1];  cb = wp[2];
	    n -= 3;
	    while (n > 0) {
		wp += 3;
		op += 3;
		n -= 3;
		op[0] = (uint16)((cr += wp[0]) & mask);
		op[1] = (uint16)((cg += wp[1]) & mask);
		op[2] = (uint16)((cb += wp[2]) & mask);
	    }
	} else if (stride == 4) {
	    op[0] = wp[0];  op[1] = wp[1];
	    op[2] = wp[2];  op[3] = wp[3];
            cr = wp[0]; cg = wp[1]; cb = wp[2]; ca = wp[3];
	    n -= 4;
	    while (n > 0) {
		wp += 4;
		op += 4;
		n -= 4;
		op[0] = (uint16)((cr += wp[0]) & mask);
		op[1] = (uint16)((cg += wp[1]) & mask);
		op[2] = (uint16)((cb += wp[2]) & mask);
		op[3] = (uint16)((ca += wp[3]) & mask);
	    } 
	} else {
	    REPEAT(stride, *op = *wp&mask; wp++; op++)
	    n -= stride;
	    while (n > 0) {
		REPEAT(stride,
		    wp[stride] += *wp; *op = *wp&mask; wp++; op++)
		n -= stride;
	    }
	}
    }
}
