static void PNGCBAPI
pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check = 0;
   png_voidp io_ptr;
   


   io_ptr = png_get_io_ptr(png_ptr);
   if (io_ptr != NULL)
   {
      check = fread(data, 1, length, (png_FILE_p)io_ptr);
   }
   if (check != length)
   {
      png_error(png_ptr, "Read Error");
   }
#ifdef PNG_IO_STATE_SUPPORTED
   pngtest_check_io_state(png_ptr, length, PNG_IO_READING);
#endif
}
static void PNGCBAPI
pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;
   png_byte *n_data;
   png_FILE_p io_ptr;
   
   n_data = (png_byte *)CVT_PTR_NOCHECK(data);
   io_ptr = (png_FILE_p)CVT_PTR(png_get_io_ptr(png_ptr));
   if ((png_bytep)n_data == data)
   {
      check = fread(n_data, 1, length, io_ptr);
   }
   else
   {
      png_byte buf[NEAR_BUF_SIZE];
      png_size_t read, remaining, err;
      check = 0;
      remaining = length;
      do
      {
         read = MIN(NEAR_BUF_SIZE, remaining);
         err = fread(buf, 1, 1, io_ptr);
         png_memcpy(data, buf, read); 
         if (err != read)
            break;
         else
            check += err;
         data += read;
         remaining -= read;
      }
      while (remaining != 0);
   }
   if (check != length)
      png_error(png_ptr, "Read Error");
#ifdef PNG_IO_STATE_SUPPORTED
   pngtest_check_io_state(png_ptr, length, PNG_IO_READING);
#endif
}
