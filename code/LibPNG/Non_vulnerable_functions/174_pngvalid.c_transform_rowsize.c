static size_t
transform_rowsize(png_const_structp pp, png_byte colour_type,
   png_byte bit_depth)
{
   return (TRANSFORM_WIDTH * bit_size(pp, colour_type, bit_depth)) / 8;
}
