void PNGCBAPI
png_default_flush(png_structp png_ptr)
{
   png_FILE_p io_ptr;
   if (png_ptr == NULL)
      return;
   io_ptr = (png_FILE_p)CVT_PTR((png_ptr->io_ptr));
   fflush(io_ptr);
}
