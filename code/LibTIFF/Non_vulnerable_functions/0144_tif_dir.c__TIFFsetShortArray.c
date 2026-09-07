    { setByteArray((void**) cpp, (void*) cp, n, 1); }
void _TIFFsetShortArray(uint16** wpp, uint16* wp, uint32 n)
    { setByteArray((void**) wpp, (void*) wp, n, sizeof (uint16)); }
