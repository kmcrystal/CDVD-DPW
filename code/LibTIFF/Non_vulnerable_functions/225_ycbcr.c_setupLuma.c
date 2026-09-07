static float*
setupLuma(float c)
{
    float *v = (float *)_TIFFmalloc(256 * sizeof (float));
    int i;
    for (i = 0; i < 256; i++)
	v[i] = c * i;
    return (v);
}
