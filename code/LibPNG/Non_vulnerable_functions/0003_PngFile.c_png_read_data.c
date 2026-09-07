static void
png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;
   


   check = (png_size_t)fread(data, (png_size_t)1, length,
      (FILE *)png_ptr->io_ptr);
   if (check != length)
   {
      png_error(png_ptr, "Read Error");
   }
}
