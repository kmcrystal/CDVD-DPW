static int
mirrorImage(uint16 spp, uint16 bps, uint16 mirror, uint32 width, uint32 length, unsigned char *ibuff)
  {
  int      shift_width;
  uint32   bytes_per_pixel, bytes_per_sample;
  uint32   row, rowsize, row_offset;
  unsigned char *line_buff = NULL;
  unsigned char *src;
  unsigned char *dst;
  src = ibuff;
  rowsize = ((width * bps * spp) + 7) / 8;
  switch (mirror)
    {
    case MIRROR_BOTH:
    case MIRROR_VERT: 
             line_buff = (unsigned char *)_TIFFmalloc(rowsize);
             if (line_buff == NULL)
               {
	       TIFFError ("mirrorImage", "Unable to allocate mirror line buffer of %1u bytes", rowsize);
               return (-1);
               }
             dst = ibuff + (rowsize * (length - 1));
             for (row = 0; row < length / 2; row++)
               {
	      _TIFFmemcpy(line_buff, src, rowsize);
	      _TIFFmemcpy(src, dst,  rowsize);
	      _TIFFmemcpy(dst, line_buff, rowsize);
               src += (rowsize);
               dst -= (rowsize);                                 
               }
             if (line_buff)
               _TIFFfree(line_buff);
             if (mirror == MIRROR_VERT)
               break;
    case MIRROR_HORIZ :
              if ((bps % 8) == 0) 
                { 
                for (row = 0; row < length; row++)
                  {
		  row_offset = row * rowsize;
                  src = ibuff + row_offset;
                  dst = ibuff + row_offset + rowsize;
                  if (reverseSamplesBytes(spp, bps, width, src, dst))
                    {
		    return (-1);
                    }
		  }
		}
	      else
                { 
                if (!(line_buff = (unsigned char *)_TIFFmalloc(rowsize + 1)))
                  {
                  TIFFError("mirrorImage", "Unable to allocate mirror line buffer");
                  return (-1);
                  }
                bytes_per_sample = (bps + 7) / 8;
                bytes_per_pixel  = ((bps * spp) + 7) / 8;
                if (bytes_per_pixel < (bytes_per_sample + 1))
                  shift_width = bytes_per_pixel;
                else
                  shift_width = bytes_per_sample + 1;
                for (row = 0; row < length; row++)
                  {
		  row_offset = row * rowsize;
                  src = ibuff + row_offset;
                  _TIFFmemset (line_buff, '\0', rowsize);
                  switch (shift_width)
                    {
                    case 1: if (reverseSamples16bits(spp, bps, width, src, line_buff))
                              {
		              _TIFFfree(line_buff);
                              return (-1);
                              }
                             _TIFFmemcpy (src, line_buff, rowsize);
                             break;
                    case 2: if (reverseSamples24bits(spp, bps, width, src, line_buff))
                              {
		              _TIFFfree(line_buff);
                              return (-1);
                              }
                             _TIFFmemcpy (src, line_buff, rowsize);
                             break;
                    case 3: 
                    case 4: 
                    case 5: if (reverseSamples32bits(spp, bps, width, src, line_buff))
                              {
		              _TIFFfree(line_buff);
                              return (-1);
                              }
                             _TIFFmemcpy (src, line_buff, rowsize);
                             break;
                    default: TIFFError("mirrorImage","Unsupported bit depth %d", bps);
		             _TIFFfree(line_buff);
                             return (-1);      
                    }
		  }
                if (line_buff)
                  _TIFFfree(line_buff);
		}
             break;
    default: TIFFError ("mirrorImage", "Invalid mirror axis %d", mirror);
             return (-1);
             break;
    }
  return (0);
  }
