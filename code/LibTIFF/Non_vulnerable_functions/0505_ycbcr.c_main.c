int
main(int argc, char** argv)
{
    int R, G, B;
    if (argc > 1) {
	refBlackWhite[0] = 16;
	refBlackWhite[1] = 235;
	refBlackWhite[2] = 128;
	refBlackWhite[3] = 240;
	refBlackWhite[4] = 128;
	refBlackWhite[5] = 240;
    }
    D3 = 2 - 2*LumaRed;
    D4 = 2 - 2*LumaBlue;
    D1 = 1. / D3;
    D2 = 1. / D4;
    D5 = D3*LumaRed / LumaGreen;
    D6 = D4*LumaBlue / LumaGreen;
    setupLumaTables();
    for (R = 0; R < 256; R++) {
	for (G = 0; G < 256; G++)
	    for (B = 0; B < 256; B++)
		check(R, G, B);
	printf("[%3u] c %u/%u b %u/%u (R %u/%d/%u G %u/%d/%u B %u/%d/%u)\n"
	    , R
	    , eCodes - preveCodes, eCodes
	    , eBits - preveBits, eBits
	    , abs(AbseRtotal - preveRtotal), eRtotal , AbseRtotal
	    , abs(AbseGtotal - preveGtotal), eGtotal , AbseGtotal
	    , abs(AbseBtotal - preveBtotal), eBtotal , AbseBtotal
	);
	preveRtotal = AbseRtotal;
	preveGtotal = AbseGtotal;
	preveBtotal = AbseBtotal;
	preveCodes = eCodes;
	preveBits = eBits;
    }
    printf("%u total codes\n", 256*256*256);
    printf("total error: %u codes %u bits (R %d/%u G %d/%u B %d/%u)\n"
	, eCodes
	, eBits
	, eRtotal , AbseRtotal
	, eGtotal , AbseGtotal
	, eBtotal , AbseBtotal
    );
    return (0);
}
