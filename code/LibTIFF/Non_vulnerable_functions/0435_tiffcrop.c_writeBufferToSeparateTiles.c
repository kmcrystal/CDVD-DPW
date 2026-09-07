 */
static int writeBufferToSeparateTiles (TIFF* out, uint8* buf, uint32 imagelength,
				       uint32 imagewidth, tsample_t spp, 
                                       struct dump_opts * dump)
  {
  tdata_t obuf = _TIFFmalloc(TIFFTileSize(out));
  uint32 tl, tw;
  uint32 row, col, nrow, ncol;
  uint32 src_rowsize, col_offset;
  uint16 bps;
  tsample_t s;
  uint8* bufp = (uint8*) buf;
  if (obuf == NULL)
    return 1;
  TIFFGetField(out, TIFFTAG_TILELENGTH, &tl);
  TIFFGetField(out, TIFFTAG_TILEWIDTH, &tw);
  TIFFGetField(out, TIFFTAG_BITSPERSAMPLE, &bps);
  if( imagewidth == 0 ||
      (uint32)bps * (uint32)spp > TIFF_UINT32_MAX / imagewidth ||
      bps * spp * imagewidth > TIFF_UINT32_MAX - 7 )
  {
      TIFFError(TIFFFileName(out),
            "Error, uint32 overflow when computing (imagewidth * bps * spp) + 7");
      _TIFFfree(obuf);
      return 1;
  }
  src_rowsize = ((imagewidth * spp * bps) + 7U) / 8;
  for (row = 0; row < imagelength; row += tl)
    {
    nrow = (row + tl > imagelength) ? imagelength - row : tl;
    for (col = 0; col < imagewidth; col += tw)
      {
      
      if (col + tw > imagewidth)
	ncol = imagewidth - col;
      else
        ncol = tw;
      col_offset = (((col * bps * spp) + 7) / 8);
      bufp = buf + (row * src_rowsize) + col_offset;
      for (s = 0; s < spp; s++)
        {
	if (extractContigSamplesToTileBuffer(obuf, bufp, nrow, ncol, imagewidth,
					     tw, s, 1, spp, bps, dump) > 0)
          {
	  TIFFError("writeBufferToSeparateTiles", 
                    "Unable to extract data to tile for row %lu, col %lu sample %d",
                    (unsigned long) row, (unsigned long)col, (int)s);
	  _TIFFfree(obuf);
	  return 1;
          }
	if (TIFFWriteTile(out, obuf, col, row, 0, s) < 0)
          {
	   TIFFError("writeBufferToseparateTiles",
	             "Cannot write tile at %lu %lu sample %lu",
	             (unsigned long) col, (unsigned long) row,
	             (unsigned long) s);
	   _TIFFfree(obuf);
	   return 1;
	  }
	}
      }
    }
  _TIFFfree(obuf);
  return 0;
  } 
