int
TIFFVGetFieldDefaulted(TIFF* tif, uint32 tag, va_list ap)
{
	TIFFDirectory *td = &tif->tif_dir;
	if (TIFFVGetField(tif, tag, ap))
		return (1);
	switch (tag) {
	case TIFFTAG_SUBFILETYPE:
		*va_arg(ap, uint32 *) = td->td_subfiletype;
		return (1);
	case TIFFTAG_BITSPERSAMPLE:
		*va_arg(ap, uint16 *) = td->td_bitspersample;
		return (1);
	case TIFFTAG_THRESHHOLDING:
		*va_arg(ap, uint16 *) = td->td_threshholding;
		return (1);
	case TIFFTAG_FILLORDER:
		*va_arg(ap, uint16 *) = td->td_fillorder;
		return (1);
	case TIFFTAG_ORIENTATION:
		*va_arg(ap, uint16 *) = td->td_orientation;
		return (1);
	case TIFFTAG_SAMPLESPERPIXEL:
		*va_arg(ap, uint16 *) = td->td_samplesperpixel;
		return (1);
	case TIFFTAG_ROWSPERSTRIP:
		*va_arg(ap, uint32 *) = td->td_rowsperstrip;
		return (1);
	case TIFFTAG_MINSAMPLEVALUE:
		*va_arg(ap, uint16 *) = td->td_minsamplevalue;
		return (1);
	case TIFFTAG_MAXSAMPLEVALUE:
		*va_arg(ap, uint16 *) = td->td_maxsamplevalue;
		return (1);
	case TIFFTAG_PLANARCONFIG:
		*va_arg(ap, uint16 *) = td->td_planarconfig;
		return (1);
	case TIFFTAG_RESOLUTIONUNIT:
		*va_arg(ap, uint16 *) = td->td_resolutionunit;
		return (1);
	case TIFFTAG_PREDICTOR:
                {
			TIFFPredictorState* sp = (TIFFPredictorState*) tif->tif_data;
			*va_arg(ap, uint16*) = (uint16) sp->predictor;
			return 1;
                }
	case TIFFTAG_DOTRANGE:
		*va_arg(ap, uint16 *) = 0;
		*va_arg(ap, uint16 *) = (1<<td->td_bitspersample)-1;
		return (1);
	case TIFFTAG_INKSET:
		*va_arg(ap, uint16 *) = INKSET_CMYK;
		return 1;
	case TIFFTAG_NUMBEROFINKS:
		*va_arg(ap, uint16 *) = 4;
		return (1);
	case TIFFTAG_EXTRASAMPLES:
		*va_arg(ap, uint16 *) = td->td_extrasamples;
		*va_arg(ap, uint16 **) = td->td_sampleinfo;
		return (1);
	case TIFFTAG_MATTEING:
		*va_arg(ap, uint16 *) =
		    (td->td_extrasamples == 1 &&
		     td->td_sampleinfo[0] == EXTRASAMPLE_ASSOCALPHA);
		return (1);
	case TIFFTAG_TILEDEPTH:
		*va_arg(ap, uint32 *) = td->td_tiledepth;
		return (1);
	case TIFFTAG_DATATYPE:
		*va_arg(ap, uint16 *) = td->td_sampleformat-1;
		return (1);
	case TIFFTAG_SAMPLEFORMAT:
		*va_arg(ap, uint16 *) = td->td_sampleformat;
                return(1);
	case TIFFTAG_IMAGEDEPTH:
		*va_arg(ap, uint32 *) = td->td_imagedepth;
		return (1);
	case TIFFTAG_YCBCRCOEFFICIENTS:
		{
			
			static float ycbcrcoeffs[] = { 0.299f, 0.587f, 0.114f };
			*va_arg(ap, float **) = ycbcrcoeffs;
			return 1;
		}
	case TIFFTAG_YCBCRSUBSAMPLING:
		*va_arg(ap, uint16 *) = td->td_ycbcrsubsampling[0];
		*va_arg(ap, uint16 *) = td->td_ycbcrsubsampling[1];
		return (1);
	case TIFFTAG_YCBCRPOSITIONING:
		*va_arg(ap, uint16 *) = td->td_ycbcrpositioning;
		return (1);
	case TIFFTAG_WHITEPOINT:
		{
			static float whitepoint[2];
			


			whitepoint[0] =	D50_X0 / (D50_X0 + D50_Y0 + D50_Z0);
			whitepoint[1] =	D50_Y0 / (D50_X0 + D50_Y0 + D50_Z0);
			*va_arg(ap, float **) = whitepoint;
			return 1;
		}
	case TIFFTAG_TRANSFERFUNCTION:
		if (!td->td_transferfunction[0] &&
		    !TIFFDefaultTransferFunction(td)) {
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "No space for \"TransferFunction\" tag");
			return (0);
		}
		*va_arg(ap, uint16 **) = td->td_transferfunction[0];
		if (td->td_samplesperpixel - td->td_extrasamples > 1) {
			*va_arg(ap, uint16 **) = td->td_transferfunction[1];
			*va_arg(ap, uint16 **) = td->td_transferfunction[2];
		}
		return (1);
	case TIFFTAG_REFERENCEBLACKWHITE:
		if (!td->td_refblackwhite && !TIFFDefaultRefBlackWhite(td))
			return (0);
		*va_arg(ap, float **) = td->td_refblackwhite;
		return (1);
	}
	return 0;
}
