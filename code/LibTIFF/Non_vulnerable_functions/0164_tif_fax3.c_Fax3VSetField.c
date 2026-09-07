static int
Fax3VSetField(TIFF* tif, ttag_t tag, va_list ap)
{
	Fax3BaseState* sp = Fax3State(tif);
	assert(sp != 0);
	assert(sp->vsetparent != 0);
	switch (tag) {
	case TIFFTAG_FAXMODE:
		sp->mode = va_arg(ap, int);
		return (1);			
	case TIFFTAG_FAXFILLFUNC:
		DecoderState(tif)->fill = va_arg(ap, TIFFFaxFillFunc);
		return (1);			
	case TIFFTAG_GROUP3OPTIONS:
		
		if (tif->tif_dir.td_compression == COMPRESSION_CCITTFAX3)
			sp->groupoptions = va_arg(ap, uint32);
		break;
	case TIFFTAG_GROUP4OPTIONS:
		
		if (tif->tif_dir.td_compression == COMPRESSION_CCITTFAX4)
			sp->groupoptions = va_arg(ap, uint32);
		break;
	case TIFFTAG_BADFAXLINES:
		sp->badfaxlines = va_arg(ap, uint32);
		break;
	case TIFFTAG_CLEANFAXDATA:
		sp->cleanfaxdata = (uint16) va_arg(ap, int);
		break;
	case TIFFTAG_CONSECUTIVEBADFAXLINES:
		sp->badfaxrun = va_arg(ap, uint32);
		break;
	case TIFFTAG_FAXRECVPARAMS:
		sp->recvparams = va_arg(ap, uint32);
		break;
	case TIFFTAG_FAXSUBADDRESS:
		_TIFFsetString(&sp->subaddress, va_arg(ap, char*));
		break;
	case TIFFTAG_FAXRECVTIME:
		sp->recvtime = va_arg(ap, uint32);
		break;
	case TIFFTAG_FAXDCS:
		_TIFFsetString(&sp->faxdcs, va_arg(ap, char*));
		break;
	default:
		return (*sp->vsetparent)(tif, tag, ap);
	}
	TIFFSetFieldBit(tif, _TIFFFieldWithTag(tif, tag)->field_bit);
	tif->tif_flags |= TIFF_DIRTYDIRECT;
	return (1);
}
