void 
png_read_data(png_structrp png_ptr, png_bytep data, png_size_t length)
{
   png_debug1(4, "reading %d bytes", (int)length);
   if (png_ptr->read_data_fn != NULL)
      (*(png_ptr->read_data_fn))(png_ptr, data, length);
   else
      png_error(png_ptr, "Call to NULL read function");
}
