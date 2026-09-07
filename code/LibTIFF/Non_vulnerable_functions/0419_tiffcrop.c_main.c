int
main(int argc, char* argv[])
  {
#if !HAVE_DECL_OPTARG
  extern int optind;
#endif
  uint16 defconfig = (uint16) -1;
  uint16 deffillorder = 0;
  uint32 deftilewidth = (uint32) 0;
  uint32 deftilelength = (uint32) 0;
  uint32 defrowsperstrip = (uint32) 0;
  uint32 dirnum = 0;
  TIFF *in = NULL;
  TIFF *out = NULL;
  char  mode[10];
  char *mp = mode;
  
  struct image_data image;     
  struct crop_mask  crop;      
  struct pagedef    page;      
  struct pageseg    sections[MAX_SECTIONS];  
  struct buffinfo   seg_buffs[MAX_SECTIONS]; 
  struct dump_opts  dump;                  
  unsigned char *read_buff    = NULL;      
  unsigned char *crop_buff    = NULL;      
  unsigned char *sect_buff    = NULL;      
  unsigned char *sect_src     = NULL;      
  unsigned int  imagelist[MAX_IMAGES + 1]; 
  unsigned int  image_count  = 0;
  unsigned int  dump_images  = 0;
  unsigned int  next_image   = 0;
  unsigned int  next_page    = 0;
  unsigned int  total_pages  = 0;
  unsigned int  total_images = 0;
  unsigned int  end_of_input = FALSE;
  int    seg, length;
  char   temp_filename[PATH_MAX + 1];
  little_endian = *((unsigned char *)&little_endian) & '1';
  initImageData(&image);
  initCropMasks(&crop);
  initPageSetup(&page, sections, seg_buffs);
  initDumpOptions(&dump);
  process_command_opts (argc, argv, mp, mode, &dirnum, &defconfig, 
                        &deffillorder, &deftilewidth, &deftilelength, &defrowsperstrip,
	                &crop, &page, &dump, imagelist, &image_count);
  if (argc - optind < 2)
    usage();
  if ((argc - optind) == 2)
    pageNum = -1;
  else
    total_images = 0;
  
  while (optind < argc - 1)
    {
    in = TIFFOpen (argv[optind], "r");
    if (in == NULL)
      return (-3);
    
    total_images = TIFFNumberOfDirectories(in); 
    if (image_count == 0)
      {
      dirnum = 0;
      total_pages = total_images; 
      }
    else
      {
      dirnum = (tdir_t)(imagelist[next_image] - 1);
      next_image++;
      


      if (image_count >  total_images)
	image_count = total_images;
      total_pages = image_count;
      }
    
    if (dirnum == (MAX_IMAGES - 1))
      dirnum = total_images - 1;
    if (dirnum > (total_images))
      {
      TIFFError (TIFFFileName(in), 
      "Invalid image number %d, File contains only %d images", 
		 (int)dirnum + 1, total_images);
      if (out != NULL)
        (void) TIFFClose(out);
      return (1);
      }
    if (dirnum != 0 && !TIFFSetDirectory(in, (tdir_t)dirnum))
      {
      TIFFError(TIFFFileName(in),"Error, setting subdirectory at %d", dirnum);
      if (out != NULL)
        (void) TIFFClose(out);
      return (1);
      }
    end_of_input = FALSE;
    while (end_of_input == FALSE)
      {
      config = defconfig;
      compression = defcompression;
      predictor = defpredictor;
      fillorder = deffillorder;
      rowsperstrip = defrowsperstrip;
      tilewidth = deftilewidth;
      tilelength = deftilelength;
      g3opts = defg3opts;
      if (dump.format != DUMP_NONE)
        {
        
	dump_images++;
        length = strlen(dump.infilename);
        if (length > 0)
          {
          if (dump.infile != NULL)
            fclose (dump.infile);
          
 
          snprintf(temp_filename, sizeof(temp_filename), "%s-read-%03d.%s",
		   dump.infilename, dump_images,
                  (dump.format == DUMP_TEXT) ? "txt" : "raw");
          if ((dump.infile = fopen(temp_filename, dump.mode)) == NULL)
            {
	    TIFFError ("Unable to open dump file for writing", "%s", temp_filename);
	    exit (-1);
            }
          dump_info(dump.infile, dump.format, "Reading image","%d from %s", 
                    dump_images, TIFFFileName(in));
          } 
        length = strlen(dump.outfilename);
        if (length > 0)
          {
          if (dump.outfile != NULL)
            fclose (dump.outfile);
          
 
          snprintf(temp_filename, sizeof(temp_filename), "%s-write-%03d.%s",
		   dump.outfilename, dump_images,
                  (dump.format == DUMP_TEXT) ? "txt" : "raw");
          if ((dump.outfile = fopen(temp_filename, dump.mode)) == NULL)
            {
	      TIFFError ("Unable to open dump file for writing", "%s", temp_filename);
	    exit (-1);
            }
          dump_info(dump.outfile, dump.format, "Writing image","%d from %s", 
                    dump_images, TIFFFileName(in));
          } 
        }
      if (dump.debug)
         TIFFError("main", "Reading image %4d of %4d total pages.", dirnum + 1, total_pages);
      if (loadImage(in, &image, &dump, &read_buff))
        {
        TIFFError("main", "Unable to load source image");
        exit (-1);
        }
      

      if (image.adjustments != 0)
        {
	if (correct_orientation(&image, &read_buff))
	    TIFFError("main", "Unable to correct image orientation");
        }
      if (getCropOffsets(&image, &crop, &dump))
        {
        TIFFError("main", "Unable to define crop regions");
        exit (-1);
	}
      if (crop.selections > 0)
        {
        if (processCropSelections(&image, &crop, &read_buff, seg_buffs))
          {
          TIFFError("main", "Unable to process image selections");
          exit (-1);
	  }
	}
      else  
        {
        if (createCroppedImage(&image, &crop, &read_buff, &crop_buff))
          {
          TIFFError("main", "Unable to create output image");
          exit (-1);
	  }
	}
      if (page.mode == PAGE_MODE_NONE)
        {  
        if (crop.selections > 0)
          {
	  writeSelections(in, &out, &crop, &image, &dump, seg_buffs,
                          mp, argv[argc - 1], &next_page, total_pages);
          }
	else  
          {
	  if (update_output_file (&out, mp, crop.exp_mode, argv[argc - 1],
                                  &next_page))
             exit (1);
          if (writeCroppedImage(in, out, &image, &dump,crop.combined_width, 
                                crop.combined_length, crop_buff, next_page, total_pages))
            {
             TIFFError("main", "Unable to write new image");
             exit (-1);
	    }
          }
	}
      else
        {
	


	if (crop_buff != NULL)  
	  sect_src = crop_buff;
        else
          sect_src = read_buff;
        
        if (computeOutputPixelOffsets(&crop, &image, &page, sections, &dump))
          {
          TIFFError("main", "Unable to compute output section data");
          exit (-1);
	  }
        


	if (update_output_file (&out, mp, crop.exp_mode, argv[argc - 1], &next_page))
          exit (1);
	if (writeImageSections(in, out, &image, &page, sections, &dump, sect_src, &sect_buff))
          {
          TIFFError("main", "Unable to write image sections");
          exit (-1);
	  }
        }
      
      if (image_count == 0)
        dirnum++;
      else
        {
	dirnum = (tdir_t)(imagelist[next_image] - 1);
        next_image++;
        }
      if (dirnum == MAX_IMAGES - 1)
        dirnum = TIFFNumberOfDirectories(in) - 1;
      if (!TIFFSetDirectory(in, (tdir_t)dirnum))
        end_of_input = TRUE;
      }
    TIFFClose(in);
    optind++;
    }
  
  if (read_buff)
    _TIFFfree(read_buff);
  if (crop_buff)
    _TIFFfree(crop_buff);
  if (sect_buff)
    _TIFFfree(sect_buff);
   
  for (seg = 0; seg < crop.selections; seg++)
    _TIFFfree (seg_buffs[seg].buffer);
  if (dump.format != DUMP_NONE)
    {
    if (dump.infile != NULL)
     fclose (dump.infile);
    if (dump.outfile != NULL)
      {
      dump_info (dump.outfile, dump.format, "", "Completed run for %s", TIFFFileName(out));
      fclose (dump.outfile);
      }
    }
  TIFFClose(out);
  return (0);
  } 
