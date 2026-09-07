static int
computeOutputPixelOffsets (struct crop_mask *crop, struct image_data *image,
                           struct pagedef *page, struct pageseg *sections,
                           struct dump_opts* dump)
  {
  double scale;
  double pwidth, plength;          
  uint32 iwidth, ilength;          
  uint32 owidth, olength;          
  uint32 orows, ocols;             
  uint32 hmargin, vmargin;         
  uint32 x1, x2, y1, y2, line_bytes;
  
  uint32 i, j, k;
  scale = 1.0;
  if (page->res_unit == RESUNIT_NONE)
    page->res_unit = image->res_unit;
  switch (image->res_unit) {
    case RESUNIT_CENTIMETER:
         if (page->res_unit == RESUNIT_INCH)
	   scale = 1.0/2.54;
	 break;
    case RESUNIT_INCH:
	 if (page->res_unit == RESUNIT_CENTIMETER)
	     scale = 2.54;
	 break;
    case RESUNIT_NONE: 
    default:
    break;
    }
  
  if (crop->combined_width > 0)
    iwidth = crop->combined_width;
  else
    iwidth = image->width;
  if (crop->combined_length > 0)
    ilength = crop->combined_length;
  else
    ilength = image->length;
  if (page->hres <= 1.0)
    page->hres = image->xres;
  if (page->vres <= 1.0)
    page->vres = image->yres;
  if ((page->hres < 1.0) || (page->vres < 1.0))
    {
    TIFFError("computeOutputPixelOffsets",
    "Invalid horizontal or vertical resolution specified or read from input image");
    return (1);
    }
  


  if (page->width <= 0)
    pwidth = iwidth;
  else
    pwidth = page->width;
  if (page->length <= 0)
    plength = ilength;
  else
    plength = page->length;
  if (dump->debug)
    {
    TIFFError("", "Page size: %s, Vres: %3.2f, Hres: %3.2f, "
                   "Hmargin: %3.2f, Vmargin: %3.2f",
	     page->name, page->vres, page->hres,
             page->hmargin, page->vmargin);
    TIFFError("", "Res_unit: %d, Scale: %3.2f, Page width: %3.2f, length: %3.2f", 
           page->res_unit, scale, pwidth, plength);
    }
  
  if (page->mode & PAGE_MODE_MARGINS)
    {
    if (page->res_unit == RESUNIT_INCH || page->res_unit == RESUNIT_CENTIMETER)
      { 
      hmargin = (uint32)(page->hmargin * scale * page->hres * ((image->bps + 7)/ 8));
      vmargin = (uint32)(page->vmargin * scale * page->vres * ((image->bps + 7)/ 8));
      }
    else
      { 
      hmargin = (uint32)(page->hmargin * scale * ((image->bps + 7)/ 8));
      vmargin = (uint32)(page->vmargin * scale * ((image->bps + 7)/ 8));
      }
    if ((hmargin * 2.0) > (pwidth * page->hres))
      {
      TIFFError("computeOutputPixelOffsets", 
                "Combined left and right margins exceed page width");
      hmargin = (uint32) 0;
      return (-1);
      }
    if ((vmargin * 2.0) > (plength * page->vres))
      {
      TIFFError("computeOutputPixelOffsets", 
                "Combined top and bottom margins exceed page length"); 
      vmargin = (uint32) 0; 
      return (-1);
      }
    }
  else
    {
    hmargin = 0;
    vmargin = 0;
    }
  if (page->mode & PAGE_MODE_ROWSCOLS )
    {
    
    if (page->mode & PAGE_MODE_MARGINS)
      TIFFError("computeOutputPixelOffsets", 
      "Output margins cannot be specified with rows and columns"); 
    owidth  = TIFFhowmany(iwidth, page->cols);
    olength = TIFFhowmany(ilength, page->rows);
    }
  else
    {
    if (page->mode & PAGE_MODE_PAPERSIZE )
      {
      owidth  = (uint32)((pwidth * page->hres) - (hmargin * 2));
      olength = (uint32)((plength * page->vres) - (vmargin * 2));
      }
    else
      {
      owidth = (uint32)(iwidth - (hmargin * 2 * page->hres));
      olength = (uint32)(ilength - (vmargin * 2 * page->vres));
      }
    }
  if (owidth > iwidth)
    owidth = iwidth;
  if (olength > ilength)
    olength = ilength;
  
  switch (page->orient)
    {
    case ORIENTATION_NONE:
    case ORIENTATION_PORTRAIT:
         ocols = TIFFhowmany(iwidth, owidth);
         orows = TIFFhowmany(ilength, olength);
         
         break;
    case ORIENTATION_LANDSCAPE:
         ocols = TIFFhowmany(iwidth, olength);
         orows = TIFFhowmany(ilength, owidth);
         x1 = olength;
         olength = owidth;
         owidth = x1;
         
         break;
    case ORIENTATION_AUTO:
    default:
         x1 = TIFFhowmany(iwidth, owidth);
         x2 = TIFFhowmany(ilength, olength); 
         y1 = TIFFhowmany(iwidth, olength);
         y2 = TIFFhowmany(ilength, owidth); 
         if ( (x1 * x2) < (y1 * y2))
           { 
           ocols = x1;
           orows = x2;
           
	   }
         else
           { 
           ocols = y1;
           orows = y2;
           x1 = olength;
           olength = owidth;
           owidth = x1;
           
           }
    }
  if (ocols < 1)
    ocols = 1;
  if (orows < 1)
    orows = 1;
  
  if (page->rows < 1)
    page->rows = orows;
  if (page->cols < 1)
    page->cols = ocols;
  line_bytes = TIFFhowmany8(owidth * image->bps) * image->spp;
  if ((page->rows * page->cols) > MAX_SECTIONS)
   {
   TIFFError("computeOutputPixelOffsets",
	     "Rows and Columns exceed maximum sections\nIncrease resolution or reduce sections");
   return (-1);
   }
  
  for (k = 0, i = 0 && k <= MAX_SECTIONS; i < orows; i++)
    {
    y1 = (uint32)(olength * i);
    y2 = (uint32)(olength * (i +  1) - 1);
    if (y2 >= ilength)
      y2 = ilength - 1;
    for (j = 0; j < ocols; j++, k++)
      {
      x1 = (uint32)(owidth * j); 
      x2 = (uint32)(owidth * (j + 1) - 1);
      if (x2 >= iwidth)
        x2 = iwidth - 1;
      sections[k].x1 = x1;
      sections[k].x2 = x2;
      sections[k].y1 = y1;
      sections[k].y2 = y2;
      sections[k].buffsize = line_bytes * olength;
      sections[k].position = k + 1;
      sections[k].total = orows * ocols;
      } 
    } 
  return (0);
  } 
