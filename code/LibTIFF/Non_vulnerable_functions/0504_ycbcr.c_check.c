void
check(int R, int G, int B)
{
    float Y, Cb, Cr;
    int iY, iCb, iCr;
    float rY, rCb, rCr;
    float rR, rG, rB;
    int eR, eG, eB;
    Y = lumaRed[R] + lumaGreen[G] + lumaBlue[B];
    Cb = (B - Y)*D2;
    Cr = (R - Y)*D1;
    iY = V2Code(Y, refBlackWhite[0], refBlackWhite[1], 255);
    iCb = V2Code(Cb, refBlackWhite[2], refBlackWhite[3], 127);
    iCr = V2Code(Cr, refBlackWhite[4], refBlackWhite[5], 127);
    rCb = Code2V(iCb, refBlackWhite[2], refBlackWhite[3], 127);
    rCr = Code2V(iCr, refBlackWhite[4], refBlackWhite[5], 127);
    rY = Code2V(iY, refBlackWhite[0], refBlackWhite[1], 255);
    rR = rY + rCr*D3;
    rB = rY + rCb*D4;
    rG = rY - rCb*D6 - rCr*D5;
    eR = R - CLAMP(rR,0,255);
    eG = G - CLAMP(rG,0,255);
    eB = B - CLAMP(rB,0,255);
    if (abs(eR) > 1 || abs(eG) > 1 || abs(eB) > 1) {
	printf("R %u G %u B %u", R, G, B);
	printf(" Y %g Cb %g Cr %g", Y, Cb, Cr);
	printf(" iY %u iCb %u iCr %u", iY, iCb, iCr);
	printf("\n -> Y %g Cb %g Cr %g", rY, rCb, rCr);
	printf(" R %g (%u) G %g (%u) B %g (%u) E=[%d %d %d])\n"
	    , rR, CLAMP(rR,0,255)
	    , rG, CLAMP(rG,0,255)
	    , rB, CLAMP(rB,0,255)
	    , eR, eG, eB
	);
    }
    eRtotal += eR;
    eGtotal += eG;
    eBtotal += eB;
    AbseRtotal += abs(eR);
    AbseGtotal += abs(eG);
    AbseBtotal += abs(eB);
    if (eR | eG | eB)
	eCodes++;
    eBits += abs(eR) + abs(eG) + abs(eB);
}
