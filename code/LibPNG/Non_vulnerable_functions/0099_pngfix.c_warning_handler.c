static void PNGCBAPI
warning_handler(png_structp png_ptr, png_const_charp message)
{
   struct file *file = get_control(png_ptr);
   if (file->global->warnings)
      emit_error(file, LIBPNG_WARNING_CODE, message);
}
