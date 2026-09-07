static int
createImageSection(uint32 sectsize, unsigned char **sect_buff_ptr)
  {
  unsigned  char *sect_buff = NULL;
  unsigned  char *new_buff  = NULL;
  static    uint32  prev_sectsize = 0;
  sect_buff = *sect_buff_ptr;
  if (!sect_buff)
    {
    sect_buff = (unsigned char *)_TIFFmalloc(sectsize);
    *sect_buff_ptr = sect_buff;
    _TIFFmemset(sect_buff, 0, sectsize);
    }
  else
    {
    if (prev_sectsize < sectsize)
      {
      new_buff = _TIFFrealloc(sect_buff, sectsize);
      if (!new_buff)
        {
	free (sect_buff);
        sect_buff = (unsigned char *)_TIFFmalloc(sectsize);
        }
      else
        sect_buff = new_buff;
      _TIFFmemset(sect_buff, 0, sectsize);
      }
    }
  if (!sect_buff)
    {
    TIFFError("createImageSection", "Unable to allocate/reallocate section buffer");
    return (-1);
    }
  prev_sectsize = sectsize;
  *sect_buff_ptr = sect_buff;
  return (0);
  }  
