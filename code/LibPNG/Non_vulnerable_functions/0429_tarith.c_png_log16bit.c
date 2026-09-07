}
static png_uint_32 png_log16bit(png_uint_32 x)
{
   if (x > 0)
      return (png_uint_32)floor(.5-log(x/65535.)*L2INV);
   return 0xffffffff;
}
