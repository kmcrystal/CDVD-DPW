static int
processCropSelections(struct image_data *image, struct crop_mask *crop, 
                      unsigned char **read_buff_ptr, struct buffinfo seg_buffs[])
  {
  int       i;
  uint32    width, length, total_width, total_length;
  tsize_t   cropsize;
  unsigned  char *crop_buff = NULL;
  unsigned  char *read_buff = NULL;
  unsigned  char *next_buff = NULL;
  tsize_t   prev_cropsize = 0;
  read_buff = *read_buff_ptr;
  if (crop->img_mode == COMPOSITE_IMAGES)
    {
    cropsize = crop->bufftotal;
    crop_buff = seg_buffs[0].buffer; 
    if (!crop_buff)
      crop_buff = (unsigned char *)_TIFFmalloc(cropsize);
    else
      {
      prev_cropsize = seg_buffs[0].size;
      if (prev_cropsize < cropsize)
        {
        next_buff = _TIFFrealloc(crop_buff, cropsize);
        if (! next_buff)
          {
          _TIFFfree (crop_buff);
          crop_buff = (unsigned char *)_TIFFmalloc(cropsize);
          }
        else
          crop_buff = next_buff;
        }
      }
    if (!crop_buff)
      {
      TIFFError("processCropSelections", "Unable to allocate/reallocate crop buffer");
      return (-1);
      }
    _TIFFmemset(crop_buff, 0, cropsize);
    seg_buffs[0].buffer = crop_buff;
    seg_buffs[0].size = cropsize;
    
    if (extractCompositeRegions(image, crop, read_buff, crop_buff) != 0)
      return (1);
    if (crop->crop_mode & CROP_INVERT)
      {
      switch (crop->photometric)
        {
        
        case PHOTOMETRIC_MINISWHITE:
        case PHOTOMETRIC_MINISBLACK:
	     image->photometric = crop->photometric;
	     break;
        case INVERT_DATA_ONLY:
        case INVERT_DATA_AND_TAG:
             if (invertImage(image->photometric, image->spp, image->bps, 
                             crop->combined_width, crop->combined_length, crop_buff))
               {
               TIFFError("processCropSelections", 
                         "Failed to invert colorspace for composite regions");
               return (-1);
               }
             if (crop->photometric == INVERT_DATA_AND_TAG)
               {
               switch (image->photometric)
                 {
                 case PHOTOMETRIC_MINISWHITE:
 	              image->photometric = PHOTOMETRIC_MINISBLACK;
	              break;
                 case PHOTOMETRIC_MINISBLACK:
 	              image->photometric = PHOTOMETRIC_MINISWHITE;
	              break;
                 default:
	              break;
	         }
	       }
             break;
        default: break;
        }
      }
    
    if (crop->crop_mode & CROP_MIRROR)
      {
      if (mirrorImage(image->spp, image->bps, crop->mirror, 
                      crop->combined_width, crop->combined_length, crop_buff))
        {
        TIFFError("processCropSelections", "Failed to mirror composite regions %s", 
	         (crop->rotation == MIRROR_HORIZ) ? "horizontally" : "vertically");
        return (-1);
        }
      }
    if (crop->crop_mode & CROP_ROTATE) 
      {
      if (rotateImage(crop->rotation, image, &crop->combined_width, 
                      &crop->combined_length, &crop_buff))
        {
        TIFFError("processCropSelections", 
                  "Failed to rotate composite regions by %d degrees", crop->rotation);
        return (-1);
        }
      seg_buffs[0].buffer = crop_buff;
      seg_buffs[0].size = (((crop->combined_width * image->bps + 7 ) / 8)
                            * image->spp) * crop->combined_length; 
      }
    }
  else  
    {
    total_width = total_length = 0;
    for (i = 0; i < crop->selections; i++)
      {
      cropsize = crop->bufftotal;
      crop_buff = seg_buffs[i].buffer; 
      if (!crop_buff)
        crop_buff = (unsigned char *)_TIFFmalloc(cropsize);
      else
        {
        prev_cropsize = seg_buffs[0].size;
        if (prev_cropsize < cropsize)
          {
          next_buff = _TIFFrealloc(crop_buff, cropsize);
          if (! next_buff)
            {
            _TIFFfree (crop_buff);
            crop_buff = (unsigned char *)_TIFFmalloc(cropsize);
            }
          else
            crop_buff = next_buff;
          }
        }
      if (!crop_buff)
        {
        TIFFError("processCropSelections", "Unable to allocate/reallocate crop buffer");
        return (-1);
        }
      _TIFFmemset(crop_buff, 0, cropsize);
      seg_buffs[i].buffer = crop_buff;
      seg_buffs[i].size = cropsize;
      if (extractSeparateRegion(image, crop, read_buff, crop_buff, i))
        {
	TIFFError("processCropSelections", "Unable to extract cropped region %d from image", i);
        return (-1);
        }
      width  = crop->regionlist[i].width;
      length = crop->regionlist[i].length;
      if (crop->crop_mode & CROP_INVERT)
        {
        switch (crop->photometric)
          {
          
          case PHOTOMETRIC_MINISWHITE:
          case PHOTOMETRIC_MINISBLACK:
	       image->photometric = crop->photometric;
	       break;
          case INVERT_DATA_ONLY:
          case INVERT_DATA_AND_TAG:
               if (invertImage(image->photometric, image->spp, image->bps, 
                               width, length, crop_buff))
                 {
                 TIFFError("processCropSelections", 
                           "Failed to invert colorspace for region");
                 return (-1);
                 }
               if (crop->photometric == INVERT_DATA_AND_TAG)
                 {
                 switch (image->photometric)
                   {
                   case PHOTOMETRIC_MINISWHITE:
 	                image->photometric = PHOTOMETRIC_MINISBLACK;
	                break;
                   case PHOTOMETRIC_MINISBLACK:
 	                image->photometric = PHOTOMETRIC_MINISWHITE;
	                break;
                   default:
	                break;
	           }
	         }
               break;
          default: break;
          }
        }
      if (crop->crop_mode & CROP_MIRROR)
        {
        if (mirrorImage(image->spp, image->bps, crop->mirror, 
                        width, length, crop_buff))
          {
          TIFFError("processCropSelections", "Failed to mirror crop region %s", 
	           (crop->rotation == MIRROR_HORIZ) ? "horizontally" : "vertically");
          return (-1);
          }
        }
      if (crop->crop_mode & CROP_ROTATE) 
        {
	if (rotateImage(crop->rotation, image, &crop->regionlist[i].width, 
			&crop->regionlist[i].length, &crop_buff))
          {
          TIFFError("processCropSelections", 
                    "Failed to rotate crop region by %d degrees", crop->rotation);
          return (-1);
          }
        total_width  += crop->regionlist[i].width;
        total_length += crop->regionlist[i].length;
        crop->combined_width = total_width;
        crop->combined_length = total_length;
        seg_buffs[i].buffer = crop_buff;
        seg_buffs[i].size = (((crop->regionlist[i].width * image->bps + 7 ) / 8)
                               * image->spp) * crop->regionlist[i].length; 
        }
      }
    }
  return (0);
  } 
