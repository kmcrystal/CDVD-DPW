static int
extractContigSamplesToBuffer(uint8 *out, uint8 *in, uint32 rows, uint32 cols,
  	                     tsample_t sample, uint16 spp, uint16 bps, 
                             struct dump_opts *dump)
  {
  int    shift_width, bytes_per_sample, bytes_per_pixel;
  uint32 src_rowsize, src_offset, row, first_col = 0;
  uint32 dst_rowsize, dst_offset;
  tsample_t count = 1;
  uint8 *src, *dst;
  bytes_per_sample = (bps + 7) / 8; 
  bytes_per_pixel  = ((bps * spp) + 7) / 8;
  if ((bps % 8) == 0)
    shift_width = 0;
  else
    {
    if (bytes_per_pixel < (bytes_per_sample + 1))
      shift_width = bytes_per_pixel;
    else
      shift_width = bytes_per_sample + 1;
    }
  src_rowsize = ((bps * spp * cols) + 7) / 8;
  dst_rowsize = ((bps * cols) + 7) / 8;
  if ((dump->outfile != NULL) && (dump->level == 4))
    {
    dump_info  (dump->outfile, dump->format, "extractContigSamplesToBuffer", 
                "Sample %d, %d rows", sample + 1, rows + 1);
    }
  for (row = 0; row < rows; row++)
    {
    src_offset = row * src_rowsize;
    dst_offset = row * dst_rowsize;
    src = in + src_offset;
    dst = out + dst_offset;
    
    switch (shift_width)
      {  
      case 0: if (extractContigSamplesBytes (src, dst, cols, sample,
                                             spp, bps,  count, first_col, cols))  
                return (1);
 	      break;
      case 1: if (bps == 1)
                {
                if (extractContigSamples8bits (src, dst, cols, sample,
                                               spp, bps, count, first_col, cols))
	          return (1);
	        break;
		}
	      else
                 if (extractContigSamples16bits (src, dst, cols, sample,
                                                 spp, bps, count, first_col, cols))
	         return (1);
	      break;
      case 2: if (extractContigSamples24bits (src, dst, cols, sample,
                                              spp, bps,  count, first_col, cols))
	         return (1);
	      break;
      case 3:
      case 4: 
      case 5: if (extractContigSamples32bits (src, dst, cols, sample,
                                              spp, bps,  count, first_col, cols))
	         return (1);
	      break;
      default: TIFFError ("extractContigSamplesToBuffer", "Unsupported bit depth: %d", bps);
	       return (1);
      }
    if ((dump->outfile != NULL) && (dump->level == 4))
      dump_buffer(dump->outfile, dump->format, 1, dst_rowsize, row, dst);
    }
  return (0);
  } 
