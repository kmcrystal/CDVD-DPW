static int
computeInputPixelOffsets(struct crop_mask *crop, struct image_data *image,
                         struct offset *off)
  {
  double scale;
  float xres, yres;
  

  uint32 tmargin, bmargin, lmargin, rmargin;
  uint32 startx, endx;   
  uint32 starty, endy;   
  uint32 width, length, crop_width, crop_length; 
  uint32 i, max_width, max_length, zwidth, zlength, buffsize;
  uint32 x1, x2, y1, y2;
  if (image->res_unit != RESUNIT_INCH && image->res_unit != RESUNIT_CENTIMETER)
    {
    xres = 1.0;
    yres = 1.0;
    }
  else
    {
    if (((image->xres == 0) || (image->yres == 0)) && 
         (crop->res_unit != RESUNIT_NONE) &&
	((crop->crop_mode & CROP_REGIONS) || (crop->crop_mode & CROP_MARGINS) ||
 	 (crop->crop_mode & CROP_LENGTH)  || (crop->crop_mode & CROP_WIDTH)))
      {
      TIFFError("computeInputPixelOffsets", "Cannot compute margins or fixed size sections without image resolution");
      TIFFError("computeInputPixelOffsets", "Specify units in pixels and try again");
      return (-1);
      }
    xres = image->xres;
    yres = image->yres;
    }
  
  scale = 1.0;
  switch (crop->res_unit) {
    case RESUNIT_CENTIMETER:
         if (image->res_unit == RESUNIT_INCH)
	   scale = 1.0/2.54;
	 break;
    case RESUNIT_INCH:
	 if (image->res_unit == RESUNIT_CENTIMETER)
	     scale = 2.54;
	 break;
    case RESUNIT_NONE: 
    default:
    break;
    }
  if (crop->crop_mode & CROP_REGIONS)
    {
    max_width = max_length = 0;
    for (i = 0; i < crop->regions; i++)
      {
      if ((crop->res_unit == RESUNIT_INCH) || (crop->res_unit == RESUNIT_CENTIMETER))
        {
	x1 = (uint32) (crop->corners[i].X1 * scale * xres);
	x2 = (uint32) (crop->corners[i].X2 * scale * xres);
	y1 = (uint32) (crop->corners[i].Y1 * scale * yres);
	y2 = (uint32) (crop->corners[i].Y2 * scale * yres);
        }
      else
        {
	x1 = (uint32) (crop->corners[i].X1);
	x2 = (uint32) (crop->corners[i].X2);
	y1 = (uint32) (crop->corners[i].Y1);
	y2 = (uint32) (crop->corners[i].Y2);       
	}
      if (x1 < 1)
        crop->regionlist[i].x1 = 0;
      else
        crop->regionlist[i].x1 = (uint32) (x1 - 1);
      if (x2 > image->width - 1)
        crop->regionlist[i].x2 = image->width - 1;
      else
        crop->regionlist[i].x2 = (uint32) (x2 - 1);
      zwidth  = crop->regionlist[i].x2 - crop->regionlist[i].x1 + 1; 
      if (y1 < 1)
        crop->regionlist[i].y1 = 0;
      else
        crop->regionlist[i].y1 = (uint32) (y1 - 1);
      if (y2 > image->length - 1)
        crop->regionlist[i].y2 = image->length - 1;
      else
        crop->regionlist[i].y2 = (uint32) (y2 - 1);
      zlength = crop->regionlist[i].y2 - crop->regionlist[i].y1 + 1; 
      if (zwidth > max_width)
        max_width = zwidth;
      if (zlength > max_length)
        max_length = zlength;
      buffsize = (uint32)
          (((zwidth * image->bps * image->spp + 7 ) / 8) * (zlength + 1));
      crop->regionlist[i].buffsize = buffsize;
      crop->bufftotal += buffsize;
      if (crop->img_mode == COMPOSITE_IMAGES)
        {
        switch (crop->edge_ref)
          {
          case EDGE_LEFT:
          case EDGE_RIGHT:
               crop->combined_length = zlength;
               crop->combined_width += zwidth;
               break;
          case EDGE_BOTTOM:
          case EDGE_TOP:  
          default:
               crop->combined_width = zwidth;
               crop->combined_length += zlength;
	       break;
          }
	}
      }
    return (0);
    }
  


  if (crop->crop_mode & CROP_MARGINS)
    {
    if (crop->res_unit != RESUNIT_INCH && crop->res_unit != RESUNIT_CENTIMETER)
      { 
      tmargin = (uint32)(crop->margins[0]);
      lmargin = (uint32)(crop->margins[1]);
      bmargin = (uint32)(crop->margins[2]);
      rmargin = (uint32)(crop->margins[3]);
      }
    else
      { 
      tmargin = (uint32)(crop->margins[0] * scale * yres);
      lmargin = (uint32)(crop->margins[1] * scale * xres);
      bmargin = (uint32)(crop->margins[2] * scale * yres);
      rmargin = (uint32)(crop->margins[3] * scale * xres);
      }
    if ((lmargin + rmargin) > image->width)
      {
      TIFFError("computeInputPixelOffsets", "Combined left and right margins exceed image width");
      lmargin = (uint32) 0;
      rmargin = (uint32) 0;
      return (-1);
      }
    if ((tmargin + bmargin) > image->length)
      {
      TIFFError("computeInputPixelOffsets", "Combined top and bottom margins exceed image length"); 
      tmargin = (uint32) 0; 
      bmargin = (uint32) 0;
      return (-1);
      }
    }
  else
    { 
    tmargin = (uint32) 0;
    lmargin = (uint32) 0;
    bmargin = (uint32) 0;
    rmargin = (uint32) 0;
    }
  
  if (crop->res_unit != RESUNIT_INCH && crop->res_unit != RESUNIT_CENTIMETER)
    {
    if (crop->crop_mode & CROP_WIDTH)
      width = (uint32)crop->width;
    else
      width = image->width - lmargin - rmargin;
    if (crop->crop_mode & CROP_LENGTH)
      length  = (uint32)crop->length;
    else
      length = image->length - tmargin - bmargin;
    }
  else
    {
    if (crop->crop_mode & CROP_WIDTH)
      width = (uint32)(crop->width * scale * image->xres);
    else
      width = image->width - lmargin - rmargin;
    if (crop->crop_mode & CROP_LENGTH)
      length  = (uint32)(crop->length * scale * image->yres);
    else
      length = image->length - tmargin - bmargin;
    }
  off->tmargin = tmargin;
  off->bmargin = bmargin;
  off->lmargin = lmargin;
  off->rmargin = rmargin;
  


  switch (crop->edge_ref) {
    case EDGE_BOTTOM:
         startx = lmargin;
         if ((startx + width) >= (image->width - rmargin))
           endx = image->width - rmargin - 1;
         else
           endx = startx + width - 1;
         endy = image->length - bmargin - 1;
         if ((endy - length) <= tmargin)
           starty = tmargin;
         else
           starty = endy - length + 1;
         break;
    case EDGE_RIGHT:
         endx = image->width - rmargin - 1;
         if ((endx - width) <= lmargin)
           startx = lmargin;
         else
           startx = endx - width + 1;
         starty = tmargin;
         if ((starty + length) >= (image->length - bmargin))
           endy = image->length - bmargin - 1;
         else
           endy = starty + length - 1;
         break;
    case EDGE_TOP:  
    case EDGE_LEFT:
    default:
         startx = lmargin;
         if ((startx + width) >= (image->width - rmargin))
           endx = image->width - rmargin - 1;
         else
           endx = startx + width - 1;
         starty = tmargin;
         if ((starty + length) >= (image->length - bmargin))
           endy = image->length - bmargin - 1;
         else
           endy = starty + length - 1;
         break;
    }
  off->startx = startx;
  off->starty = starty;
  off->endx   = endx;
  off->endy   = endy;
  crop_width  = endx - startx + 1;
  crop_length = endy - starty + 1;
  if (crop_width <= 0)
    {
    TIFFError("computeInputPixelOffsets", 
               "Invalid left/right margins and /or image crop width requested");
    return (-1);
    }
  if (crop_width > image->width)
    crop_width = image->width;
  if (crop_length <= 0)
    {
    TIFFError("computeInputPixelOffsets", 
              "Invalid top/bottom margins and /or image crop length requested");
    return (-1);
    }
  if (crop_length > image->length)
    crop_length = image->length;
  off->crop_width = crop_width;
  off->crop_length = crop_length;
  return (0);
  } 
