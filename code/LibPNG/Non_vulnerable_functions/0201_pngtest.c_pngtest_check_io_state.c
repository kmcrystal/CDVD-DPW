void
pngtest_check_io_state(png_structp png_ptr, png_size_t data_length,
   png_uint_32 io_op)
{
   png_uint_32 io_state = png_get_io_state(png_ptr);
   int err = 0;
   
   if ((io_state & PNG_IO_MASK_OP) != io_op)
      png_error(png_ptr, "Incorrect operation in I/O state");
   


   switch (io_state & PNG_IO_MASK_LOC)
   {
   case PNG_IO_SIGNATURE:
      if (data_length > 8)
         err = 1;
      break;
   case PNG_IO_CHUNK_HDR:
      if (data_length != 8)
         err = 1;
      break;
   case PNG_IO_CHUNK_DATA:
      break;  
   case PNG_IO_CHUNK_CRC:
      if (data_length != 4)
         err = 1;
      break;
   default:
      err = 1;  
   }
   if (err)
      png_error(png_ptr, "Bad I/O state or buffer size");
}
