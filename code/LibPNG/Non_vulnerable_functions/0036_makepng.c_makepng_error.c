static void PNGCBAPI
makepng_error(png_structp png_ptr, png_const_charp message)
{
   makepng_warning(png_ptr, message);
   png_longjmp(png_ptr, 1);
}
