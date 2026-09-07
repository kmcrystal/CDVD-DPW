    { setByteArray((void**) lpp, (void*) lp, n, sizeof (uint32)); }
void _TIFFsetFloatArray(float** fpp, float* fp, uint32 n)
    { setByteArray((void**) fpp, (void*) fp, n, sizeof (float)); }
