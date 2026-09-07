
static png_uint_32 png_log8bit(unsigned x)
{
   if (x > 0)
      return (png_uint_32)floor(.5-log(x/255.)*L2INV);
   return 0xffffffff;
}
