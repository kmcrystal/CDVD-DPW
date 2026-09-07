static int
Fax3Encode2DRow(TIFF* tif, unsigned char* bp, unsigned char* rp, uint32 bits)
{
#define	PIXEL(buf,ix)	((((buf)[(ix)>>3]) >> (7-((ix)&7))) & 1)
        uint32 a0 = 0;
	uint32 a1 = (PIXEL(bp, 0) != 0 ? 0 : finddiff(bp, 0, bits, 0));
	uint32 b1 = (PIXEL(rp, 0) != 0 ? 0 : finddiff(rp, 0, bits, 0));
	uint32 a2, b2;
	for (;;) {
		b2 = finddiff2(rp, b1, bits, PIXEL(rp,b1));
		if (b2 >= a1) {
			int32 d = b1 - a1;
			if (!(-3 <= d && d <= 3)) {	
				a2 = finddiff2(bp, a1, bits, PIXEL(bp,a1));
				putcode(tif, &horizcode);
				if (a0+a1 == 0 || PIXEL(bp, a0) == 0) {
					putspan(tif, a1-a0, TIFFFaxWhiteCodes);
					putspan(tif, a2-a1, TIFFFaxBlackCodes);
				} else {
					putspan(tif, a1-a0, TIFFFaxBlackCodes);
					putspan(tif, a2-a1, TIFFFaxWhiteCodes);
				}
				a0 = a2;
			} else {			
				putcode(tif, &vcodes[d+3]);
				a0 = a1;
			}
		} else {				
			putcode(tif, &passcode);
			a0 = b2;
		}
		if (a0 >= bits)
			break;
		a1 = finddiff(bp, a0, bits, PIXEL(bp,a0));
		b1 = finddiff(rp, a0, bits, !PIXEL(bp,a0));
		b1 = finddiff(rp, b1, bits, PIXEL(bp,a0));
	}
	return (1);
#undef PIXEL
}
