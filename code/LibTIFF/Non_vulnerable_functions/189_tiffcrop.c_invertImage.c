static int
invertImage(uint16 photometric, uint16 spp, uint16 bps, uint32 width, uint32 length, unsigned char *work_buff)
  {
  uint32   row, col;
  unsigned char  bytebuff1, bytebuff2, bytebuff3, bytebuff4;
  unsigned char *src;
  uint16        *src_uint16;
  uint32        *src_uint32;
  if (spp != 1)
    {
    TIFFError("invertImage", "Image inversion not supported for more than one sample per pixel");
    return (-1);
    }
  if (photometric !=  PHOTOMETRIC_MINISWHITE && photometric !=  PHOTOMETRIC_MINISBLACK)
    {
    TIFFError("invertImage", "Only black and white and grayscale images can be inverted");
    return (-1);
    }
  src = work_buff;
  if (src == NULL)
    {
    TIFFError ("invertImage", "Invalid crop buffer passed to invertImage");
    return (-1);
    }
  switch (bps)
    {
    case 32: src_uint32 = (uint32 *)src;
             for (row = 0; row < length; row++)
               for (col = 0; col < width; col++)
                 {
		 *src_uint32 = (uint32)0xFFFFFFFF - *src_uint32;
                  src_uint32++;
                 }
            break;
    case 16: src_uint16 = (uint16 *)src;
             for (row = 0; row < length; row++)
               for (col = 0; col < width; col++)
                 {
		 *src_uint16 = (uint16)0xFFFF - *src_uint16;
                  src_uint16++;
                 }
            break;
    case 8: for (row = 0; row < length; row++)
              for (col = 0; col < width; col++)
                {
		*src = (uint8)255 - *src;
                 src++;
                }
            break;
    case 4: for (row = 0; row < length; row++)
              for (col = 0; col < width; col++)
                {
		bytebuff1 = 16 - (uint8)(*src & 240 >> 4);
		bytebuff2 = 16 - (*src & 15);
		*src = bytebuff1 << 4 & bytebuff2;
                src++;
                }
            break;
    case 2: for (row = 0; row < length; row++)
              for (col = 0; col < width; col++)
                {
		bytebuff1 = 4 - (uint8)(*src & 192 >> 6);
		bytebuff2 = 4 - (uint8)(*src & 48  >> 4);
		bytebuff3 = 4 - (uint8)(*src & 12  >> 2);
		bytebuff4 = 4 - (uint8)(*src & 3);
		*src = (bytebuff1 << 6) || (bytebuff2 << 4) || (bytebuff3 << 2) || bytebuff4;
                src++;
                }
            break;
    case 1: for (row = 0; row < length; row++)
              for (col = 0; col < width; col += 8 /(spp * bps))
                {
                *src = ~(*src);
                src++;
                }
            break;
    default: TIFFError("invertImage", "Unsupported bit depth %d", bps);
      return (-1);
    }
  return (0);
  }
