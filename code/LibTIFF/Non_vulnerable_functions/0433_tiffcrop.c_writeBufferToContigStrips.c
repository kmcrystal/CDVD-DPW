  }
static int writeBufferToContigStrips(TIFF* out, uint8* buf, uint32 imagelength)
  {
  uint32 row, nrows, rowsperstrip;
  tstrip_t strip = 0;
  tsize_t stripsize;
  TIFFGetFieldDefaulted(out, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
  for (row = 0; row < imagelength; row += rowsperstrip)
    {
    nrows = (row + rowsperstrip > imagelength) ?
	     imagelength - row : rowsperstrip;
    stripsize = TIFFVStripSize(out, nrows);
    if (TIFFWriteEncodedStrip(out, strip++, buf, stripsize) < 0)
      {
      TIFFError(TIFFFileName(out), "Error, can't write strip %u", strip - 1);
      return 1;
      }
    buf += stripsize;
    }
  return 0;
  }
