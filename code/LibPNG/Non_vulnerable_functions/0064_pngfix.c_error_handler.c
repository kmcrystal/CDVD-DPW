static void PNGCBAPI
error_handler(png_structp png_ptr, png_const_charp message)
{
   stop(get_control(png_ptr),  LIBPNG_ERROR_CODE, message);
}
