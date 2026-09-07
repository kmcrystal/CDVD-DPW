static int
extractImageSection(struct image_data *image, struct pageseg *section, 
                    unsigned char *src_buff, unsigned char *sect_buff)
  {
  unsigned  char  bytebuff1, bytebuff2;
#ifdef DEVELMODE
  
#endif
  uint32    img_width, img_rowsize;
#ifdef DEVELMODE
  uint32    img_length;
#endif
  uint32    j, shift1, shift2, trailing_bits;
  uint32    row, first_row, last_row, first_col, last_col;
  uint32    src_offset, dst_offset, row_offset, col_offset;
  uint32    offset1, offset2, full_bytes;
  uint32    sect_width;
#ifdef DEVELMODE
  uint32    sect_length;
#endif
  uint16    bps, spp;
#ifdef DEVELMODE
  int      k;
  unsigned char bitset;
  static char *bitarray = NULL;
#endif
  img_width = image->width;
#ifdef DEVELMODE
  img_length = image->length;
#endif
  bps = image->bps;
  spp = image->spp;
#ifdef DEVELMODE
  
  
#endif
  src_offset = 0;
  dst_offset = 0;
#ifdef DEVELMODE
  if (bitarray == NULL)
    {
    if ((bitarray = (char *)malloc(img_width)) == NULL)
      {
      TIFFError ("", "DEBUG: Unable to allocate debugging bitarray");
      return (-1);
      }
    }
#endif
  
  first_row = section->y1;
  last_row  = section->y2;
  first_col = section->x1;
  last_col  = section->x2;
  sect_width = last_col - first_col + 1;
#ifdef DEVELMODE
  sect_length = last_row - first_row + 1;
#endif
  img_rowsize = ((img_width * bps + 7) / 8) * spp;
  full_bytes = (sect_width * spp * bps) / 8;   
  trailing_bits = (sect_width * bps) % 8;
#ifdef DEVELMODE
    TIFFError ("", "First row: %d, last row: %d, First col: %d, last col: %d\n",
           first_row, last_row, first_col, last_col);
    TIFFError ("", "Image width: %d, Image length: %d, bps: %d, spp: %d\n",
	   img_width, img_length, bps, spp);
    TIFFError ("", "Sect  width: %d,  Sect length: %d, full bytes: %d trailing bits %d\n", 
           sect_width, sect_length, full_bytes, trailing_bits);
#endif
  if ((bps % 8) == 0)
    {
    col_offset = first_col * spp * bps / 8;
    for (row = first_row; row <= last_row; row++)
      {
      
      row_offset = row * img_rowsize;
      src_offset = row_offset + col_offset;
#ifdef DEVELMODE
        TIFFError ("", "Src offset: %8d, Dst offset: %8d", src_offset, dst_offset); 
#endif
      _TIFFmemcpy (sect_buff + dst_offset, src_buff + src_offset, full_bytes);
      dst_offset += full_bytes;
      }        
    }
  else
    { 
    shift1  = spp * ((first_col * bps) % 8);
    shift2  = spp * ((last_col * bps) % 8);
    for (row = first_row; row <= last_row; row++)
      {
      
      row_offset = row * img_rowsize;
      offset1 = row_offset + (first_col * bps / 8);
      offset2 = row_offset + (last_col * bps / 8);
#ifdef DEVELMODE
      for (j = 0, k = 7; j < 8; j++, k--)
        {
        bitset = *(src_buff + offset1) & (((unsigned char)1 << k)) ? 1 : 0;
        sprintf(&bitarray[j], (bitset) ? "1" : "0");
        }
      sprintf(&bitarray[8], " ");
      sprintf(&bitarray[9], " ");
      for (j = 10, k = 7; j < 18; j++, k--)
        {
        bitset = *(src_buff + offset2) & (((unsigned char)1 << k)) ? 1 : 0;
        sprintf(&bitarray[j], (bitset) ? "1" : "0");
        }
      bitarray[18] = '\0';
      TIFFError ("", "Row: %3d Offset1: %d,  Shift1: %d,    Offset2: %d,  Shift2:  %d\n", 
                 row, offset1, shift1, offset2, shift2); 
#endif
      bytebuff1 = bytebuff2 = 0;
      if (shift1 == 0) 
        {
	_TIFFmemcpy (sect_buff + dst_offset, src_buff + offset1, full_bytes);
#ifdef DEVELMODE
	TIFFError ("", "        Alligned data src offset1: %8d, Dst offset: %8d\n", offset1, dst_offset); 
	sprintf(&bitarray[18], "\n");
	sprintf(&bitarray[19], "\t");
        for (j = 20, k = 7; j < 28; j++, k--)
          {
          bitset = *(sect_buff + dst_offset) & (((unsigned char)1 << k)) ? 1 : 0;
          sprintf(&bitarray[j], (bitset) ? "1" : "0");
          }
        bitarray[28] = ' ';
        bitarray[29] = ' ';
#endif
        dst_offset += full_bytes;
        if (trailing_bits != 0)
          {
	  bytebuff2 = src_buff[offset2] & ((unsigned char)255 << (7 - shift2));
          sect_buff[dst_offset] = bytebuff2;
#ifdef DEVELMODE
	  TIFFError ("", "        Trailing bits src offset:  %8d, Dst offset: %8d\n", 
                              offset2, dst_offset); 
          for (j = 30, k = 7; j < 38; j++, k--)
            {
            bitset = *(sect_buff + dst_offset) & (((unsigned char)1 << k)) ? 1 : 0;
            sprintf(&bitarray[j], (bitset) ? "1" : "0");
            }
          bitarray[38] = '\0';
          TIFFError ("", "\tFirst and last bytes before and after masking:\n\t%s\n\n", bitarray);
#endif
          dst_offset++;
          }
        }
      else   
        {
#ifdef DEVELMODE
	  TIFFError ("", "        Unalligned data src offset: %8d, Dst offset: %8d\n", offset1 , dst_offset); 
#endif
        for (j = 0; j <= full_bytes; j++) 
          {
	  bytebuff1 = src_buff[offset1 + j] & ((unsigned char)255 >> shift1);
	  bytebuff2 = src_buff[offset1 + j + 1] & ((unsigned char)255 << (7 - shift1));
          sect_buff[dst_offset + j] = (bytebuff1 << shift1) | (bytebuff2 >> (8 - shift1));
          }
#ifdef DEVELMODE
	sprintf(&bitarray[18], "\n");
	sprintf(&bitarray[19], "\t");
        for (j = 20, k = 7; j < 28; j++, k--)
          {
          bitset = *(sect_buff + dst_offset) & (((unsigned char)1 << k)) ? 1 : 0;
          sprintf(&bitarray[j], (bitset) ? "1" : "0");
          }
        bitarray[28] = ' ';
        bitarray[29] = ' ';
#endif
        dst_offset += full_bytes;
        if (trailing_bits != 0)
          {
#ifdef DEVELMODE
	    TIFFError ("", "        Trailing bits   src offset: %8d, Dst offset: %8d\n", offset1 + full_bytes, dst_offset); 
#endif
	  if (shift2 > shift1)
            {
	    bytebuff1 = src_buff[offset1 + full_bytes] & ((unsigned char)255 << (7 - shift2));
            bytebuff2 = bytebuff1 & ((unsigned char)255 << shift1);
            sect_buff[dst_offset] = bytebuff2;
#ifdef DEVELMODE
	    TIFFError ("", "        Shift2 > Shift1\n"); 
#endif
            }
          else
            {
	    if (shift2 < shift1)
              {
              bytebuff2 = ((unsigned char)255 << (shift1 - shift2 - 1));
	      sect_buff[dst_offset] &= bytebuff2;
#ifdef DEVELMODE
	      TIFFError ("", "        Shift2 < Shift1\n"); 
#endif
              }
#ifdef DEVELMODE
            else
	      TIFFError ("", "        Shift2 == Shift1\n"); 
#endif
            }
	  }
#ifdef DEVELMODE
	  sprintf(&bitarray[28], " ");
	  sprintf(&bitarray[29], " ");
          for (j = 30, k = 7; j < 38; j++, k--)
            {
            bitset = *(sect_buff + dst_offset) & (((unsigned char)1 << k)) ? 1 : 0;
            sprintf(&bitarray[j], (bitset) ? "1" : "0");
            }
          bitarray[38] = '\0';
          TIFFError ("", "\tFirst and last bytes before and after masking:\n\t%s\n\n", bitarray);
#endif
        dst_offset++;
        }
      }
    }
  return (0);
  } 
