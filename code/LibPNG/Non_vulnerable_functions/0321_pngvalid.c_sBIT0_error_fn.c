static void
sBIT0_error_fn(png_structp pp, png_infop pi)
{
   
   png_color_8 bad;
   bad.red = bad.green = bad.blue = bad.gray = bad.alpha = 0;
   png_set_sBIT(pp, pi, &bad);
}
