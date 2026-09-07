static png_uint_32
standard_width(png_const_structp pp, png_uint_32 id)
{
   png_uint_32 width = WIDTH_FROM_ID(id);
   UNUSED(pp)
   if (width == 0)
      width = transform_width(pp, COL_FROM_ID(id), DEPTH_FROM_ID(id));
   return width;
}
