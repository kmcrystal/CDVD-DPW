static int
_XTIFFVSetField(TIFF* tif, ttag_t tag, va_list ap)
{
	xtiff *xt = XTIFFDIR(tif);
	XTIFFDirectory* xd = &xt->xtif_dir;
	int status = 1;
	uint32 v32=0;
	int i=0, v=0;
	va_list ap1 = ap;
	
	switch (tag) {
		



	case TIFFTAG_EXAMPLE_MULTI:
		
		xd->xd_num_multi = (uint16) va_arg(ap, int);
		_TIFFsetDoubleArray(&xd->xd_example_multi, va_arg(ap, double*),
			(long) xd->xd_num_multi);
		break;
	case TIFFTAG_EXAMPLE_SINGLE:
		xd->xd_example_single = va_arg(ap, uint32);
		break;
	case TIFFTAG_EXAMPLE_ASCII:
		_TIFFsetString(&xd->xd_example_ascii, va_arg(ap, char*));
		break;
	default:
		
		return (PARENT(xt,vsetfield))(tif,tag,ap);
		break;
	}
	if (status) {
		





		if (!(xt->xtif_flags & XTIFFP_PRINT))
		{
	        	PARENT(xt,printdir) =  TIFFMEMBER(tif,printdir);
      		  	TIFFMEMBER(tif,printdir) = _XTIFFPrintDirectory;
			xt->xtif_flags |= XTIFFP_PRINT;
		}
		TIFFSetFieldBit(tif, _TIFFFieldWithTag(tif, tag)->field_bit);
		tif->tif_flags |= TIFF_DIRTYDIRECT;
	}
	va_end(ap);
	return (status);
badvalue:
	TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "%d: Bad value for \"%s\"", v,
	    _TIFFFieldWithTag(tif, tag)->field_name);
	va_end(ap);
	return (0);
badvalue32:
	TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "%ld: Bad value for \"%s\"", v32,
	    _TIFFFieldWithTag(tif, tag)->field_name);
	va_end(ap);
	return (0);
}
