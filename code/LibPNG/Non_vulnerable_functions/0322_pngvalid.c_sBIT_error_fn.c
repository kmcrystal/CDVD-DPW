static void
sBIT_error_fn(png_structp pp, png_infop pi)
{
   png_byte bit_depth;
   png_color_8 bad;
   if (png_get_color_type(pp, pi) == PNG_COLOR_TYPE_PALETTE)
      bit_depth = 8;
   else
      bit_depth = png_get_bit_depth(pp, pi);
   
   bad.red = bad.green = bad.blue = bad.gray = bad.alpha =
      (png_byte)(bit_depth+1);
   png_set_sBIT(pp, pi, &bad);
}
