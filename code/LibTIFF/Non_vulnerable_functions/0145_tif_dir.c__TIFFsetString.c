    { setByteArray(vpp, vp, n, 1); }
void _TIFFsetString(char** cpp, char* cp)
    { setByteArray((void**) cpp, (void*) cp, strlen(cp)+1, 1); }
