static png_uint_32
standard_rowsize(png_const_structp pp, png_uint_32 id)
{
   png_uint_32 width = standard_width(pp, id);
   
   width *= bit_size(pp, COL_FROM_ID(id), DEPTH_FROM_ID(id));
   return (width + 7) / 8;
}
