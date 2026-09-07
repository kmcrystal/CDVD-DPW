static int
processCompressOptions(char* opt)
  {
  char* cp = NULL;
  if (strneq(opt, "none",4))
    {
    defcompression = COMPRESSION_NONE;
    }
  else if (streq(opt, "packbits"))
    {
    defcompression = COMPRESSION_PACKBITS;
    }
  else if (strneq(opt, "jpeg", 4))
    {
    cp = strchr(opt, ':');
    defcompression = COMPRESSION_JPEG;
    while (cp)
      {
      if (isdigit((int)cp[1]))
	quality = atoi(cp + 1);
      else if (strneq(cp + 1, "raw", 3 ))
	jpegcolormode = JPEGCOLORMODE_RAW;
      else if (strneq(cp + 1, "rgb", 3 ))
	jpegcolormode = JPEGCOLORMODE_RGB;
      else
	usage();
      cp = strchr(cp + 1, ':');
      }
    }
  else if (strneq(opt, "g3", 2))
    {
    processG3Options(opt);
    defcompression = COMPRESSION_CCITTFAX3;
    }
  else if (streq(opt, "g4"))
    {
    defcompression = COMPRESSION_CCITTFAX4;
    }
  else if (strneq(opt, "lzw", 3))
    {
    cp = strchr(opt, ':');
    if (cp)
      defpredictor = atoi(cp+1);
    defcompression = COMPRESSION_LZW;
    }
  else if (strneq(opt, "zip", 3))
    {
    cp = strchr(opt, ':');
    if (cp)
      defpredictor = atoi(cp+1);
    defcompression = COMPRESSION_ADOBE_DEFLATE;
   }
  else
    return (0);
  return (1);
  }
