void PNGCBAPI
png_default_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;
   if (png_ptr == NULL)
      return;
   check = fwrite(data, 1, length, (png_FILE_p)(png_ptr->io_ptr));
   if (check != length)
      png_error(png_ptr, "Write Error");
}
void PNGCBAPI
png_default_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_uint_32 check;
   png_byte *near_data;  
   png_FILE_p io_ptr;
   if (png_ptr == NULL)
      return;
   
   near_data = (png_byte *)CVT_PTR_NOCHECK(data);
   io_ptr = (png_FILE_p)CVT_PTR(png_ptr->io_ptr);
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
      png_error(png_ptr, "Write Error");
}
