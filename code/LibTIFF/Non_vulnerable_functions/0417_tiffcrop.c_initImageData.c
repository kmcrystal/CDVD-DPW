static void
initImageData (struct image_data *image)
  {
  image->xres = 0.0;
  image->yres = 0.0;
  image->width = 0;
  image->length = 0;
  image->res_unit = RESUNIT_NONE;
  image->bps = 0;
  image->spp = 0;
  image->planar = 0;
  image->photometric = 0;
  image->orientation = 0;
  image->compression = COMPRESSION_NONE;
  image->adjustments = 0;
  }
