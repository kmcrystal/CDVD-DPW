static int
Fax3VGetField(TIFF* tif, ttag_t tag, va_list ap)
{
	Fax3BaseState* sp = Fax3State(tif);
	switch (tag) {
	case TIFFTAG_FAXMODE:
		*va_arg(ap, int*) = sp->mode;
		break;
	case TIFFTAG_FAXFILLFUNC:
		*va_arg(ap, TIFFFaxFillFunc*) = DecoderState(tif)->fill;
		break;
	case TIFFTAG_GROUP3OPTIONS:
	case TIFFTAG_GROUP4OPTIONS:
		*va_arg(ap, uint32*) = sp->groupoptions;
		break;
	case TIFFTAG_BADFAXLINES:
		*va_arg(ap, uint32*) = sp->badfaxlines;
		break;
	case TIFFTAG_CLEANFAXDATA:
		*va_arg(ap, uint16*) = sp->cleanfaxdata;
		break;
	case TIFFTAG_CONSECUTIVEBADFAXLINES:
		*va_arg(ap, uint32*) = sp->badfaxrun;
		break;
	case TIFFTAG_FAXRECVPARAMS:
		*va_arg(ap, uint32*) = sp->recvparams;
		break;
	case TIFFTAG_FAXSUBADDRESS:
		*va_arg(ap, char**) = sp->subaddress;
		break;
	case TIFFTAG_FAXRECVTIME:
		*va_arg(ap, uint32*) = sp->recvtime;
		break;
	case TIFFTAG_FAXDCS:
		*va_arg(ap, char**) = sp->faxdcs;
		break;
	default:
		return (*sp->vgetparent)(tif, tag, ap);
	}
	return (1);
}
