static int  
writeSingleSection(TIFF *in, TIFF *out, struct image_data *image,
                   struct dump_opts *dump, uint32 width, uint32 length,
                   double hres, double vres,
                   unsigned char *sect_buff)
  {
  uint16 bps, spp;
  uint16 input_compression, input_photometric;
  uint16 input_planar;
  struct cpTag* p;
  


  input_compression = image->compression;
  input_photometric = image->photometric;
  spp = image->spp;
  bps = image->bps;
  TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(out, TIFFTAG_IMAGELENGTH, length);
  TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, bps);
  TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, spp);
#ifdef DEBUG2
  TIFFError("writeSingleSection", "Input compression: %s",
	    (input_compression == COMPRESSION_OJPEG) ? "Old Jpeg" :
	    ((input_compression == COMPRESSION_JPEG) ?  "New Jpeg" : "Non Jpeg"));
#endif
  




  if (compression != (uint16)-1)
    TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
  else
    { 
    if (input_compression == COMPRESSION_OJPEG)
      {
      compression = COMPRESSION_JPEG;
      jpegcolormode = JPEGCOLORMODE_RAW;
      TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
      }
    else 
      CopyField(TIFFTAG_COMPRESSION, compression);
    }
  if (compression == COMPRESSION_JPEG)
    {
    if ((input_photometric == PHOTOMETRIC_PALETTE) ||  
        (input_photometric == PHOTOMETRIC_MASK))       
      {
      TIFFError ("writeSingleSection",
                 "JPEG compression cannot be used with %s image data",
		 (input_photometric == PHOTOMETRIC_PALETTE) ?
                 "palette" : "mask");
      return (-1);
      }
    if ((input_photometric == PHOTOMETRIC_RGB) &&
	(jpegcolormode == JPEGCOLORMODE_RGB))
      TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_YCBCR);
    else
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, input_photometric);
    }
  else
    {
    if (compression == COMPRESSION_SGILOG || compression == COMPRESSION_SGILOG24)
      TIFFSetField(out, TIFFTAG_PHOTOMETRIC, spp == 1 ?
			PHOTOMETRIC_LOGL : PHOTOMETRIC_LOGLUV);
    else
      TIFFSetField(out, TIFFTAG_PHOTOMETRIC, image->photometric);
    }
#ifdef DEBUG2
  TIFFError("writeSingleSection", "Input photometric: %s",
	    (input_photometric == PHOTOMETRIC_RGB) ? "RGB" :
	    ((input_photometric == PHOTOMETRIC_YCBCR) ?  "YCbCr" : "Not RGB or YCbCr"));
#endif
  if (((input_photometric == PHOTOMETRIC_LOGL) ||
       (input_photometric ==  PHOTOMETRIC_LOGLUV)) &&
      ((compression != COMPRESSION_SGILOG) && 
       (compression != COMPRESSION_SGILOG24)))
    {
    TIFFError("writeSingleSection",
              "LogL and LogLuv source data require SGI_LOG or SGI_LOG24 compression");
    return (-1);
    }
  if (fillorder != 0)
    TIFFSetField(out, TIFFTAG_FILLORDER, fillorder);
  else
    CopyTag(TIFFTAG_FILLORDER, 1, TIFF_SHORT);
  






  TIFFSetField(out, TIFFTAG_ORIENTATION, image->orientation);
  




  if (outtiled == -1)
    outtiled = TIFFIsTiled(in);
  if (outtiled) {
    





    if (tilewidth == (uint32) 0)
      TIFFGetField(in, TIFFTAG_TILEWIDTH, &tilewidth);
    if (tilelength == (uint32) 0)
      TIFFGetField(in, TIFFTAG_TILELENGTH, &tilelength);
    if (tilewidth == 0 || tilelength == 0)
      TIFFDefaultTileSize(out, &tilewidth, &tilelength);
    TIFFDefaultTileSize(out, &tilewidth, &tilelength);
    TIFFSetField(out, TIFFTAG_TILEWIDTH, tilewidth);
    TIFFSetField(out, TIFFTAG_TILELENGTH, tilelength);
    } else {
       




	if (rowsperstrip == (uint32) 0)
          {
	  if (!TIFFGetField(in, TIFFTAG_ROWSPERSTRIP, &rowsperstrip))
	    rowsperstrip = TIFFDefaultStripSize(out, rowsperstrip);
          if (compression != COMPRESSION_JPEG)
            {
  	    if (rowsperstrip > length)
	      rowsperstrip = length;
	    }
	  }
	else 
          if (rowsperstrip == (uint32) -1)
	    rowsperstrip = length;
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	}
  TIFFGetFieldDefaulted(in, TIFFTAG_PLANARCONFIG, &input_planar);
  if (config != (uint16) -1)
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, config);
  else
    CopyField(TIFFTAG_PLANARCONFIG, config);
  if (spp <= 4)
    CopyTag(TIFFTAG_TRANSFERFUNCTION, 4, TIFF_SHORT);
  CopyTag(TIFFTAG_COLORMAP, 4, TIFF_SHORT);

  switch (compression) {
    


    case COMPRESSION_JPEG:
         if (((bps % 8) == 0) || ((bps % 12) == 0))
	   {
           TIFFSetField(out, TIFFTAG_JPEGQUALITY, quality);
	   TIFFSetField(out, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
           }
         else
           {
	   TIFFError("writeSingleSection",
                     "JPEG compression requires 8 or 12 bits per sample");
           return (-1);
           }
	 break;
   case COMPRESSION_LZW:
   case COMPRESSION_ADOBE_DEFLATE:
   case COMPRESSION_DEFLATE:
	if (predictor != (uint16)-1)
          TIFFSetField(out, TIFFTAG_PREDICTOR, predictor);
	else
	  CopyField(TIFFTAG_PREDICTOR, predictor);
	break;
   case COMPRESSION_CCITTFAX3:
   case COMPRESSION_CCITTFAX4:
	if (compression == COMPRESSION_CCITTFAX3) {
          if (g3opts != (uint32) -1)
	    TIFFSetField(out, TIFFTAG_GROUP3OPTIONS, g3opts);
	  else
	    CopyField(TIFFTAG_GROUP3OPTIONS, g3opts);
	} else {
	    CopyTag(TIFFTAG_GROUP4OPTIONS, 1, TIFF_LONG);
        }
        CopyTag(TIFFTAG_BADFAXLINES, 1, TIFF_LONG);
        CopyTag(TIFFTAG_CLEANFAXDATA, 1, TIFF_LONG);
        CopyTag(TIFFTAG_CONSECUTIVEBADFAXLINES, 1, TIFF_LONG);
        CopyTag(TIFFTAG_FAXRECVPARAMS, 1, TIFF_LONG);
        CopyTag(TIFFTAG_FAXRECVTIME, 1, TIFF_LONG);
        CopyTag(TIFFTAG_FAXSUBADDRESS, 1, TIFF_ASCII);
	break;
   }
   { uint32 len32;
     void** data;
     if (TIFFGetField(in, TIFFTAG_ICCPROFILE, &len32, &data))
       TIFFSetField(out, TIFFTAG_ICCPROFILE, len32, data);
   }
   { uint16 ninks;
     const char* inknames;
     if (TIFFGetField(in, TIFFTAG_NUMBEROFINKS, &ninks)) {
       TIFFSetField(out, TIFFTAG_NUMBEROFINKS, ninks);
       if (TIFFGetField(in, TIFFTAG_INKNAMES, &inknames)) {
	 int inknameslen = strlen(inknames) + 1;
	 const char* cp = inknames;
	 while (ninks > 1) {
	   cp = strchr(cp, '\0');
	   if (cp) {
	     cp++;
	     inknameslen += (strlen(cp) + 1);
	   }
	   ninks--;
         }
	 TIFFSetField(out, TIFFTAG_INKNAMES, inknameslen, inknames);
       }
     }
   }
   {
   unsigned short pg0, pg1;
   if (TIFFGetField(in, TIFFTAG_PAGENUMBER, &pg0, &pg1)) {
     if (pageNum < 0) 
	TIFFSetField(out, TIFFTAG_PAGENUMBER, pg0, pg1);
     else 
	TIFFSetField(out, TIFFTAG_PAGENUMBER, pageNum++, 0);
     }
   }
  for (p = tags; p < &tags[NTAGS]; p++)
		CopyTag(p->tag, p->count, p->type);
  
  TIFFSetField(out, TIFFTAG_XRESOLUTION, (float)hres);
  TIFFSetField(out, TIFFTAG_YRESOLUTION, (float)vres);
  
  if (outtiled)
    {
    if (config == PLANARCONFIG_CONTIG)
      writeBufferToContigTiles (out, sect_buff, length, width, spp, dump);
    else
      writeBufferToSeparateTiles (out, sect_buff, length, width, spp, dump);
    }
  else
    {
    if (config == PLANARCONFIG_CONTIG)
      writeBufferToContigStrips (out, sect_buff, length);
    else
      writeBufferToSeparateStrips(out, sect_buff, length, width, spp, dump);
    }
  if (!TIFFWriteDirectory(out))
    {
    TIFFClose(out);
    return (-1);
    }
  return (0);
  } 
