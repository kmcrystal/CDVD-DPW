static int 
writeSelections(TIFF *in, TIFF **out, struct crop_mask *crop, 
                struct image_data *image, struct dump_opts *dump,
                struct buffinfo seg_buffs[], char *mp, char *filename, 
                unsigned int *page, unsigned int total_pages)
  {
  int i, page_count;
  int autoindex = 0;
  unsigned char *crop_buff = NULL;
    
  switch (crop->exp_mode)
    {
    case ONE_FILE_COMPOSITE: 
         autoindex = 0;
         crop_buff = seg_buffs[0].buffer;
         if (update_output_file (out, mp, autoindex, filename, page))
           return (1);
         page_count = total_pages;
         if (writeCroppedImage(in, *out, image, dump,
                               crop->combined_width, 
                               crop->combined_length,
                               crop_buff, *page, total_pages))
            {
             TIFFError("writeRegions", "Unable to write new image");
             return (-1);
             }
	 break;
    case ONE_FILE_SEPARATED: 
         autoindex = 0;
         if (update_output_file (out, mp, autoindex, filename, page))
           return (1);
         page_count = crop->selections * total_pages;
         for (i = 0; i < crop->selections; i++)
           {
           crop_buff = seg_buffs[i].buffer;
           if (writeCroppedImage(in, *out, image, dump,
                                 crop->regionlist[i].width, 
                                 crop->regionlist[i].length, 
                                 crop_buff, *page, page_count))
             {
             TIFFError("writeRegions", "Unable to write new image");
             return (-1);
             }
	   }
         break;
    case FILE_PER_IMAGE_COMPOSITE: 
         autoindex = 1;
         if (update_output_file (out, mp, autoindex, filename, page))
           return (1);
         crop_buff = seg_buffs[0].buffer;
         if (writeCroppedImage(in, *out, image, dump,
                               crop->combined_width, 
                               crop->combined_length, 
                               crop_buff, *page, total_pages))
           {
           TIFFError("writeRegions", "Unable to write new image");
           return (-1);
           }
         break;
    case FILE_PER_IMAGE_SEPARATED: 
         autoindex = 1;
         page_count = crop->selections;
         if (update_output_file (out, mp, autoindex, filename, page))
           return (1);
         for (i = 0; i < crop->selections; i++)
           {
           crop_buff = seg_buffs[i].buffer;
           
           if (writeCroppedImage(in, *out, image, dump,
                                 crop->regionlist[i].width, 
                                 crop->regionlist[i].length, 
                                 crop_buff, *page, page_count))
             {
             TIFFError("writeRegions", "Unable to write new image");
             return (-1);
             }
           }
         break;
    case FILE_PER_SELECTION:
         autoindex = 1;
	 page_count = 1;
         for (i = 0; i < crop->selections; i++)
           {
           if (update_output_file (out, mp, autoindex, filename, page))
             return (1);
           crop_buff = seg_buffs[i].buffer;
           
           if (writeCroppedImage(in, *out, image, dump,
                                 crop->regionlist[i].width, 
                                 crop->regionlist[i].length, 
                                 crop_buff, *page, page_count))
             {
             TIFFError("writeRegions", "Unable to write new image");
             return (-1);
             }
           }
	 break;
    default: return (1);
    }
  return (0);
  } 
