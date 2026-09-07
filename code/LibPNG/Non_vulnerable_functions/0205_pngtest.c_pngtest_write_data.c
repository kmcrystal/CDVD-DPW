static void PNGCBAPI
pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;
   check = fwrite(data, 1, length, (png_FILE_p)png_get_io_ptr(png_ptr));
   if (check != length)
   {
      png_error(png_ptr, "Write Error");
   }
#ifdef PNG_IO_STATE_SUPPORTED
   pngtest_check_io_state(png_ptr, length, PNG_IO_WRITING);
#endif
}
static void PNGCBAPI
pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;
   png_byte *near_data;  
   png_FILE_p io_ptr;
   
   near_data = (png_byte *)CVT_PTR_NOCHECK(data);
   io_ptr = (png_FILE_p)CVT_PTR(png_get_io_ptr(png_ptr));
   if ((png_bytep)near_data == data)
   {
      check = fwrite(near_data, 1, length, io_ptr);
   }
   else
   {
      png_byte buf[NEAR_BUF_SIZE];
      png_size_t written, remaining, err;
      check = 0;
      remaining = length;
      do
      {
         written = MIN(NEAR_BUF_SIZE, remaining);
         png_memcpy(buf, data, written); 
         err = fwrite(buf, 1, written, io_ptr);
         if (err != written)
            break;
         else
            check += err;
         data += written;
         remaining -= written;
      }
      while (remaining != 0);
   }
   if (check != length)
   {
      png_error(png_ptr, "Write Error");
   }
#ifdef PNG_IO_STATE_SUPPORTED
   pngtest_check_io_state(png_ptr, length, PNG_IO_WRITING);
#endif
}
