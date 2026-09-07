static png_uint_32
standard_height(png_const_structp pp, png_uint_32 id)
{
   png_uint_32 height = HEIGHT_FROM_ID(id);
   if (height == 0)
      height = transform_height(pp, COL_FROM_ID(id), DEPTH_FROM_ID(id));
   return height;
}
