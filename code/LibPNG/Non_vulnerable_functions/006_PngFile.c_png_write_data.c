static void
png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_uint_32 check;
   check = fwrite(data, 1, length, (FILE *)(png_ptr->io_ptr));
   if (check != length)
   {
      png_error(png_ptr, "Write Error");
   }
}
