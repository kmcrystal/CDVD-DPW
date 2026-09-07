int
TIFFImageIterBegin(TIFFImageIter* img, TIFF* tif, int stop, char emsg[1024])
{
    uint16* sampleinfo;
    uint16 extrasamples;
    uint16 planarconfig;
    int colorchannels;
    img->tif = tif;
    img->stoponerr = stop;
    TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &img->bitspersample);
    img->alpha = 0;
    TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &img->samplesperpixel);
    TIFFGetFieldDefaulted(tif, TIFFTAG_EXTRASAMPLES,
	&extrasamples, &sampleinfo);
    if (extrasamples == 1)
	switch (sampleinfo[0]) {
	case EXTRASAMPLE_ASSOCALPHA:	
	case EXTRASAMPLE_UNASSALPHA:	
	    img->alpha = sampleinfo[0];
	    break;
	}
    colorchannels = img->samplesperpixel - extrasamples;
    TIFFGetFieldDefaulted(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    if (!TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &img->photometric)) {
	switch (colorchannels) {
	case 1:
	    if (isCCITTCompression(tif))
		img->photometric = PHOTOMETRIC_MINISWHITE;
	    else
		img->photometric = PHOTOMETRIC_MINISBLACK;
	    break;
	case 3:
	    img->photometric = PHOTOMETRIC_RGB;
	    break;
	default:
	    sprintf(emsg, "Missing needed %s tag", photoTag);
	    return (0);
	}
    }
    switch (img->photometric) {
    case PHOTOMETRIC_PALETTE:
	if (!TIFFGetField(tif, TIFFTAG_COLORMAP,
	    &img->redcmap, &img->greencmap, &img->bluecmap)) {
		TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), "Missing required \"Colormap\" tag");
	    return (0);
	}
	
    case PHOTOMETRIC_MINISWHITE:
    case PHOTOMETRIC_MINISBLACK:









	break;
    case PHOTOMETRIC_YCBCR:
	if (planarconfig != PLANARCONFIG_CONTIG) {
	    sprintf(emsg, "Sorry, can not handle YCbCr images with %s=%d",
		"Planarconfiguration", planarconfig);
	    return (0);
	}
	
	{ uint16 compress;
	  TIFFGetField(tif, TIFFTAG_COMPRESSION, &compress);
	  if (compress == COMPRESSION_JPEG && planarconfig == PLANARCONFIG_CONTIG) {
	    
	    
	    TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
	    img->photometric = PHOTOMETRIC_RGB;
	  }
	}
	break;
    case PHOTOMETRIC_RGB: 
	if (colorchannels < 3) {
	    sprintf(emsg, "Sorry, can not handle RGB image with %s=%d",
		"Color channels", colorchannels);
	    return (0);
	}
	break;
    case PHOTOMETRIC_SEPARATED: {
	uint16 inkset;
	TIFFGetFieldDefaulted(tif, TIFFTAG_INKSET, &inkset);
	if (inkset != INKSET_CMYK) {
	    sprintf(emsg, "Sorry, can not handle separated image with %s=%d",
		"InkSet", inkset);
	    return (0);
	}
	if (img->samplesperpixel != 4) {
	    sprintf(emsg, "Sorry, can not handle separated image with %s=%d",
		"Samples/pixel", img->samplesperpixel);
	    return (0);
	}
	break;
    }
    default:
	sprintf(emsg, "Sorry, can not handle image with %s=%d",
	    photoTag, img->photometric);
	return (0);
    }
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &img->width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &img->height);
    TIFFGetFieldDefaulted(tif, TIFFTAG_ORIENTATION, &img->orientation);
    switch (img->orientation) {
    case ORIENTATION_BOTRIGHT:
    case ORIENTATION_RIGHTBOT:	
    case ORIENTATION_LEFTBOT:	
	TIFFWarning(TIFFFileName(tif), "using bottom-left orientation");
	img->orientation = ORIENTATION_BOTLEFT;
	
    case ORIENTATION_BOTLEFT:
	break;
    case ORIENTATION_TOPRIGHT:
    case ORIENTATION_RIGHTTOP:	
    case ORIENTATION_LEFTTOP:	
    default:
	TIFFWarning(TIFFFileName(tif), "using top-left orientation");
	img->orientation = ORIENTATION_TOPLEFT;
	
    case ORIENTATION_TOPLEFT:
	break;
    }
    img->isContig =
	!(planarconfig == PLANARCONFIG_SEPARATE && colorchannels > 1);
    if (img->isContig) {
	img->get = TIFFIsTiled(tif) ? gtTileContig : gtStripContig;
    } else {
	img->get = TIFFIsTiled(tif) ? gtTileSeparate : gtStripSeparate;
    }
    return (1);
}
