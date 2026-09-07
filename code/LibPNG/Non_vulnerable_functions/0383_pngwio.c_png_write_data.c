void 
png_write_data(png_structp png_ptr, png_const_bytep data, png_size_t length)
{
   
   if (png_ptr->write_data_fn != NULL )
      (*(png_ptr->write_data_fn))(png_ptr, (png_bytep)data, length);
   else
      png_error(png_ptr, "Call to NULL write function");
}
