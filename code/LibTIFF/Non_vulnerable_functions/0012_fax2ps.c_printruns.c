static void
printruns(unsigned char* buf, uint32* runs, uint32* erun, uint32 lastx)
{
    static struct {
	char white, black;
	unsigned short width;
    } WBarr[] = {
	{ 'd', 'n', 512 }, { 'e', 'o', 256 }, { 'f', 'p', 128 },
	{ 'g', 'q',  64 }, { 'h', 'r',  32 }, { 'i', 's',  16 },
	{ 'j', 't',   8 }, { 'k', 'u',   4 }, { 'l', 'v',   2 },
	{ 'm', 'w',   1 }
    };
    static char* svalue =
	" !\"#$&'*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abc";
    int colormode = 1;		
    uint32 runlength = 0;
    int n = maxline;
    uint32 x = 0;
    int l;
    (void) buf;
    printf("%d m(", row++);
    while (runs < erun) {
	if (runlength <= 0) {
	    colormode ^= 1;
	    runlength = *runs++;
	    if (x+runlength > lastx)
		runlength = runs[-1] = lastx-x;
	    x += runlength;
	    if (!colormode && runs == erun)	
		break;		
	}
	









	l = 0;
	while (runlength > 6) {	
	    if (runlength >= WBarr[l].width) {
		if (n == 0) {
		    putchar('\n');
		    n = maxline;
		}
		putchar(colormode ? WBarr[l].black : WBarr[l].white), n--;
		runlength -= WBarr[l].width;
	    } else
		l++;
	}
	while (runlength > 0 && runlength <= 6) {
	    uint32 bitsleft = 6;
	    int t = 0;
	    while (bitsleft) {
		if (runlength <= bitsleft) {
		    if (colormode)
			t |= ((1 << runlength)-1) << (bitsleft-runlength);
		    bitsleft -= runlength;
		    runlength = 0;
		    if (bitsleft) {
			if (runs >= erun)
			    break;
			colormode ^= 1;
			runlength = *runs++;
			if (x+runlength > lastx)
			    runlength = runs[-1] = lastx-x;
			x += runlength;
		    }
		} else {		
		    if (colormode)
			t |= ((1 << bitsleft)-1);
		    runlength -= bitsleft;
		    bitsleft = 0;
		}
	    }
	    if (n == 0) {
		putchar('\n');
		n = maxline;
	    }
	    putchar(svalue[t]), n--;
	}
    }
    printf(")s\n");
}
