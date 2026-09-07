static int
_XTIFFVGetField(TIFF* tif, ttag_t tag, va_list ap)
{
	xtiff *xt = XTIFFDIR(tif);
	XTIFFDirectory* xd = &xt->xtif_dir;
	switch (tag) {
		



	case TIFFTAG_EXAMPLE_MULTI:
		*va_arg(ap, uint16*) = xd->xd_num_multi;
		*va_arg(ap, double**) = xd->xd_example_multi;
		break;
	case TIFFTAG_EXAMPLE_ASCII:
		*va_arg(ap, char**) = xd->xd_example_ascii;
		break;
	case TIFFTAG_EXAMPLE_SINGLE:
		*va_arg(ap, uint32*) = xd->xd_example_single;
		break;
	default:
		
		return (PARENT(xt,vgetfield))(tif,tag,ap);
		break;
	}
	return (1);
}
