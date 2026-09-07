static void
modifier_crc(png_bytep buffer)
{
   


   uInt datalen = png_get_uint_32(buffer);
   uLong crc = crc32(0, buffer+4, datalen+4);
   

   png_save_uint_32(buffer+datalen+8, (png_uint_32)crc);
}
