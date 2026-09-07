static int
getCropOffsets(struct image_data *image, struct crop_mask *crop, struct dump_opts *dump)
  {
  struct offset offsets;
  int    i;
  int32  test;
  uint32 seg, total, need_buff = 0;
  uint32 buffsize;
  uint32 zwidth, zlength;
  memset(&offsets, '\0', sizeof(struct offset));
  crop->bufftotal = 0;
  crop->combined_width  = (uint32)0;
  crop->combined_length = (uint32)0;
  crop->selections = 0;
  
  if ((crop->crop_mode & CROP_MARGINS) ||
      (crop->crop_mode & CROP_REGIONS) ||
      (crop->crop_mode & CROP_LENGTH)  || 
      (crop->crop_mode & CROP_WIDTH))
    {
    if (computeInputPixelOffsets(crop, image, &offsets))
      {
      TIFFError ("getCropOffsets", "Unable to compute crop margins");
      return (-1);
      }
    need_buff = TRUE;
    crop->selections = crop->regions;
    
    if (crop->crop_mode & CROP_REGIONS)
      return (0);
    }
  else
    { 
    offsets.tmargin = 0;
    offsets.lmargin = 0;
    offsets.bmargin = 0;
    offsets.rmargin = 0;
    offsets.crop_width = image->width;
    offsets.crop_length = image->length;
    offsets.startx = 0;
    offsets.endx = image->width - 1;
    offsets.starty = 0;
    offsets.endy = image->length - 1;
    need_buff = FALSE;
    }
  if (dump->outfile != NULL)
    {
    dump_info (dump->outfile, dump->format, "", "Margins: Top: %d  Left: %d  Bottom: %d  Right: %d", 
           offsets.tmargin, offsets.lmargin, offsets.bmargin, offsets.rmargin); 
    dump_info (dump->outfile, dump->format, "", "Crop region within margins: Adjusted Width:  %6d  Length: %6d", 
           offsets.crop_width, offsets.crop_length);
    }
  if (!(crop->crop_mode & CROP_ZONES)) 
    {
    if (need_buff == FALSE)  
      {
      crop->selections = 0;
      crop->combined_width  = image->width;
      crop->combined_length = image->length;
      return (0);
      }
    else 
      {
      


      crop->selections = 1;
      crop->zones = 1;
      crop->zonelist[0].total = 1;
      crop->zonelist[0].position = 1;
      }
    }     
  else
    crop->selections = crop->zones;
  for (i = 0; i < crop->zones; i++)
    {
    seg = crop->zonelist[i].position;
    total = crop->zonelist[i].total;
    switch (crop->edge_ref) 
      {
      case EDGE_LEFT: 
           zlength = offsets.crop_length;
	   crop->regionlist[i].y1 = offsets.starty;
           crop->regionlist[i].y2 = offsets.endy;
           crop->regionlist[i].x1 = offsets.startx + 
                                  (uint32)(offsets.crop_width * 1.0 * (seg - 1) / total);
           test = (int32)offsets.startx + 
                  (int32)(offsets.crop_width * 1.0 * seg / total);
           if (test < 1 )
             crop->regionlist[i].x2 = 0;
           else
	     {
	     if (test > (int32)(image->width - 1))
               crop->regionlist[i].x2 = image->width - 1;
             else
	       crop->regionlist[i].x2 = test - 1;
             }
           zwidth = crop->regionlist[i].x2 - crop->regionlist[i].x1  + 1;
	   
           crop->combined_length = (uint32)zlength;
           if (crop->exp_mode == COMPOSITE_IMAGES)
             crop->combined_width += (uint32)zwidth;
           else
             crop->combined_width = (uint32)zwidth;
           break;
      case EDGE_BOTTOM: 
           zwidth = offsets.crop_width;
	   crop->regionlist[i].x1 = offsets.startx;
           crop->regionlist[i].x2 = offsets.endx;
           test = offsets.endy - (uint32)(offsets.crop_length * 1.0 * seg / total);
           if (test < 1 )
	     crop->regionlist[i].y1 = 0;
           else
	     crop->regionlist[i].y1 = test + 1;
           test = offsets.endy - (offsets.crop_length * 1.0 * (seg - 1) / total);
           if (test < 1 )
             crop->regionlist[i].y2 = 0;
           else
	     {
             if (test > (int32)(image->length - 1))
               crop->regionlist[i].y2 = image->length - 1;
             else 
               crop->regionlist[i].y2 = test;
	     }
           zlength = crop->regionlist[i].y2 - crop->regionlist[i].y1 + 1;
	   
           if (crop->exp_mode == COMPOSITE_IMAGES)
             crop->combined_length += (uint32)zlength;
           else
             crop->combined_length = (uint32)zlength;
           crop->combined_width = (uint32)zwidth;
           break;
      case EDGE_RIGHT: 
           zlength = offsets.crop_length;
	   crop->regionlist[i].y1 = offsets.starty;
           crop->regionlist[i].y2 = offsets.endy;
           crop->regionlist[i].x1 = offsets.startx +
                                  (uint32)(offsets.crop_width  * (total - seg) * 1.0 / total);
           test = offsets.startx + 
	          (offsets.crop_width * (total - seg + 1) * 1.0 / total);
           if (test < 1 )
             crop->regionlist[i].x2 = 0;
           else
	     {
	     if (test > (int32)(image->width - 1))
               crop->regionlist[i].x2 = image->width - 1;
             else
               crop->regionlist[i].x2 = test - 1;
             }
           zwidth = crop->regionlist[i].x2 - crop->regionlist[i].x1  + 1;
	   
           crop->combined_length = (uint32)zlength;
           if (crop->exp_mode == COMPOSITE_IMAGES)
             crop->combined_width += (uint32)zwidth;
           else
             crop->combined_width = (uint32)zwidth;
           break;
      case EDGE_TOP: 
      default:
           zwidth = offsets.crop_width;
	   crop->regionlist[i].x1 = offsets.startx;
           crop->regionlist[i].x2 = offsets.endx;
           crop->regionlist[i].y1 = offsets.starty + (uint32)(offsets.crop_length * 1.0 * (seg - 1) / total);
           test = offsets.starty + (uint32)(offsets.crop_length * 1.0 * seg / total);
           if (test < 1 )
             crop->regionlist[i].y2 = 0;
           else
	     {
	     if (test > (int32)(image->length - 1))
	       crop->regionlist[i].y2 = image->length - 1;
             else
	       crop->regionlist[i].y2 = test - 1;
	     }
           zlength = crop->regionlist[i].y2 - crop->regionlist[i].y1 + 1;
	   
           if (crop->exp_mode == COMPOSITE_IMAGES)
             crop->combined_length += (uint32)zlength;
           else
             crop->combined_length = (uint32)zlength;
           crop->combined_width = (uint32)zwidth;
           break;
      } 
    buffsize = (uint32)
          ((((zwidth * image->bps * image->spp) + 7 ) / 8) * (zlength + 1));
    crop->regionlist[i].width = (uint32) zwidth;
    crop->regionlist[i].length = (uint32) zlength;
    crop->regionlist[i].buffsize = buffsize;
    crop->bufftotal += buffsize;
  if (dump->outfile != NULL)
    dump_info (dump->outfile, dump->format, "",  "Zone %d, width: %4d, length: %4d, x1: %4d  x2: %4d  y1: %4d  y2: %4d",
                    i + 1, (uint32)zwidth, (uint32)zlength,
		    crop->regionlist[i].x1, crop->regionlist[i].x2, 
                    crop->regionlist[i].y1, crop->regionlist[i].y2);
    }
  return (0);
  } 
