static int
writeImageSections(TIFF *in, TIFF *out, struct image_data *image,
		   struct pagedef *page, struct pageseg *sections,
		   struct dump_opts * dump, unsigned char *src_buff,
                   unsigned char **sect_buff_ptr)
  {
  double  hres, vres;
  uint32  i, k, width, length, sectsize;
  unsigned char *sect_buff = *sect_buff_ptr;
  hres = page->hres;
  vres = page->vres;
  k = page->cols * page->rows;
  if ((k < 1) || (k > MAX_SECTIONS))
   {
   TIFFError("writeImageSections",
	     "%d Rows and Columns exceed maximum sections\nIncrease resolution or reduce sections", k);
   return (-1);
   }
  for (i = 0; i < k; i++)
    {
    width  = sections[i].x2 - sections[i].x1 + 1;
    length = sections[i].y2 - sections[i].y1 + 1;
    sectsize = (uint32)
	    ceil((width * image->bps + 7) / (double)8) * image->spp * length;
    
    if (createImageSection(sectsize, sect_buff_ptr))
      {
      TIFFError("writeImageSections", "Unable to allocate section buffer");
      exit (-1);
      }
    sect_buff = *sect_buff_ptr;
    if (extractImageSection (image, &sections[i], src_buff, sect_buff))
      {
      TIFFError("writeImageSections", "Unable to extract image sections");
      exit (-1);
      }
  
    if (writeSingleSection(in, out, image, dump, width, length, hres, vres, sect_buff))
      {
      TIFFError("writeImageSections", "Unable to write image section");
      exit (-1);
      }
    }
  return (0);
  } 
