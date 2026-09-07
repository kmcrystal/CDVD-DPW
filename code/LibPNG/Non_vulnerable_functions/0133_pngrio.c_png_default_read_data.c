void PNGCBAPI
png_default_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;
   if (png_ptr == NULL)
      return;
   


   check = fread(data, 1, length, png_voidcast(png_FILE_p, png_ptr->io_ptr));
   if (check != length)
      png_error(png_ptr, "Read Error");
}
