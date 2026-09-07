static int
PixarLogMakeTables(PixarLogState *sp)
{










    int  nlin, lt2size;
    int  i, j;
    double  b, c, linstep, v;
    float *ToLinearF;
    uint16 *ToLinear16;
    unsigned char *ToLinear8;
    uint16  *FromLT2;
    uint16  *From14; 
    uint16  *From8;
    c = log(RATIO);	
    nlin = (int)(1./c);	
    c = 1./nlin;
    b = exp(-c*ONE);	
    linstep = b*c*exp(1.);
    LogK1 = (float)(1./c);	
    LogK2 = (float)(1./b);
    lt2size = (int)(2./linstep) + 1;
    FromLT2 = (uint16 *)_TIFFmalloc(lt2size*sizeof(uint16));
    From14 = (uint16 *)_TIFFmalloc(16384*sizeof(uint16));
    From8 = (uint16 *)_TIFFmalloc(256*sizeof(uint16));
    ToLinearF = (float *)_TIFFmalloc(TSIZEP1 * sizeof(float));
    ToLinear16 = (uint16 *)_TIFFmalloc(TSIZEP1 * sizeof(uint16));
    ToLinear8 = (unsigned char *)_TIFFmalloc(TSIZEP1 * sizeof(unsigned char));
    if (FromLT2 == NULL || From14  == NULL || From8   == NULL ||
	 ToLinearF == NULL || ToLinear16 == NULL || ToLinear8 == NULL) {
	if (FromLT2) _TIFFfree(FromLT2);
	if (From14) _TIFFfree(From14);
	if (From8) _TIFFfree(From8);
	if (ToLinearF) _TIFFfree(ToLinearF);
	if (ToLinear16) _TIFFfree(ToLinear16);
	if (ToLinear8) _TIFFfree(ToLinear8);
	sp->FromLT2 = NULL;
	sp->From14 = NULL;
	sp->From8 = NULL;
	sp->ToLinearF = NULL;
	sp->ToLinear16 = NULL;
	sp->ToLinear8 = NULL;
	return 0;
    }
    j = 0;
    for (i = 0; i < nlin; i++)  {
	v = i * linstep;
	ToLinearF[j++] = (float)v;
    }
    for (i = nlin; i < TSIZE; i++)
	ToLinearF[j++] = (float)(b*exp(c*i));
    ToLinearF[2048] = ToLinearF[2047];
    for (i = 0; i < TSIZEP1; i++)  {
	v = ToLinearF[i]*65535.0 + 0.5;
	ToLinear16[i] = (v > 65535.0) ? 65535 : (uint16)v;
	v = ToLinearF[i]*255.0  + 0.5;
	ToLinear8[i]  = (v > 255.0) ? 255 : (unsigned char)v;
    }
    j = 0;
    for (i = 0; i < lt2size; i++)  {
	if ((i*linstep)*(i*linstep) > ToLinearF[j]*ToLinearF[j+1])
	    j++;
	FromLT2[i] = (uint16)j;
    }
    




    j = 0;
    for (i = 0; i < 16384; i++)  {
	while ((i/16383.)*(i/16383.) > ToLinearF[j]*ToLinearF[j+1])
	    j++;
	From14[i] = (uint16)j;
    }
    j = 0;
    for (i = 0; i < 256; i++)  {
	while ((i/255.)*(i/255.) > ToLinearF[j]*ToLinearF[j+1])
	    j++;
	From8[i] = (uint16)j;
    }
    Fltsize = (float)(lt2size/2);
    sp->ToLinearF = ToLinearF;
    sp->ToLinear16 = ToLinear16;
    sp->ToLinear8 = ToLinear8;
    sp->FromLT2 = FromLT2;
    sp->From14 = From14;
    sp->From8 = From8;
    return 1;
}
