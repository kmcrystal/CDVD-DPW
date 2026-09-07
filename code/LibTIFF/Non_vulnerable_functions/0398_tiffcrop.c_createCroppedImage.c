static int
createCroppedImage(struct image_data *image, struct crop_mask *crop, 
                   unsigned char **read_buff_ptr, unsigned char **crop_buff_ptr)
  {
  tsize_t   cropsize;
  unsigned  char *read_buff = NULL;
  unsigned  char *crop_buff = NULL;
  unsigned  char *new_buff  = NULL;
  static    tsize_t  prev_cropsize = 0;
  read_buff = *read_buff_ptr;
  
  crop_buff = read_buff;
  *crop_buff_ptr = read_buff;
  crop->combined_width = image->width;
  crop->combined_length = image->length;
  cropsize = crop->bufftotal;
  crop_buff = *crop_buff_ptr;
  if (!crop_buff)
    {
    crop_buff = (unsigned char *)_TIFFmalloc(cropsize);
    *crop_buff_ptr = crop_buff;
    _TIFFmemset(crop_buff, 0, cropsize);
    prev_cropsize = cropsize;
    }
  else
    {
    if (prev_cropsize < cropsize)
      {
      new_buff = _TIFFrealloc(crop_buff, cropsize);
      if (!new_buff)
        {
	free (crop_buff);
        crop_buff = (unsigned char *)_TIFFmalloc(cropsize);
        }
      else
        crop_buff = new_buff;
      _TIFFmemset(crop_buff, 0, cropsize);
      }
    }
  if (!crop_buff)
    {
    TIFFError("createCroppedImage", "Unable to allocate/reallocate crop buffer");
    return (-1);
    }
  *crop_buff_ptr = crop_buff;
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
             TIFFError("createCroppedImage", 
                       "Failed to invert colorspace for image or cropped selection");
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
      TIFFError("createCroppedImage", "Failed to mirror image or cropped selection %s", 
	       (crop->rotation == MIRROR_HORIZ) ? "horizontally" : "vertically");
      return (-1);
      }
    }
  if (crop->crop_mode & CROP_ROTATE) 
    {
    if (rotateImage(crop->rotation, image, &crop->combined_width, 
                    &crop->combined_length, crop_buff_ptr))
      {
      TIFFError("createCroppedImage", 
                "Failed to rotate image or cropped selection by %d degrees", crop->rotation);
      return (-1);
      }
    }
  if (crop_buff == read_buff) 
    *read_buff_ptr = NULL;    
  return (0);
  } 
