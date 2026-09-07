static void
processG3Options(char* cp)
{
	g3opts = 0;
        if( (cp = strchr(cp, ':')) ) {
                do {
                        cp++;
                        if (strneq(cp, "1d", 2))
                                g3opts &= ~GROUP3OPT_2DENCODING;
                        else if (strneq(cp, "2d", 2))
                                g3opts |= GROUP3OPT_2DENCODING;
                        else if (strneq(cp, "fill", 4))
                                g3opts |= GROUP3OPT_FILLBITS;
                        else
                                usage();
                } while( (cp = strchr(cp, ':')) );
        }
}
