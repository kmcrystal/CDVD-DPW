static int
PixarLogVGetField(TIFF* tif, uint32 tag, va_list ap)
{
    PixarLogState *sp = (PixarLogState *)tif->tif_data;
    switch (tag) {
     case TIFFTAG_PIXARLOGQUALITY:
	*va_arg(ap, int*) = sp->quality;
	break;
     case TIFFTAG_PIXARLOGDATAFMT:
	*va_arg(ap, int*) = sp->user_datafmt;
	break;
     default:
	return (*sp->vgetparent)(tif, tag, ap);
    }
    return (1);
}
