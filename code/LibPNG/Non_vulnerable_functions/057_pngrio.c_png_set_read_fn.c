void PNGAPI
png_set_read_fn(png_structrp png_ptr, png_voidp io_ptr,
   png_rw_ptr read_data_fn)
{
   if (png_ptr == NULL)
      return;
   png_ptr->io_ptr = io_ptr;
   if (read_data_fn != NULL)
      png_ptr->read_data_fn = read_data_fn;
   else
      png_ptr->read_data_fn = png_default_read_data;
   png_ptr->read_data_fn = read_data_fn;
   
   if (png_ptr->write_data_fn != NULL)
   {
      png_ptr->write_data_fn = NULL;
      png_warning(png_ptr,
          "Can't set both read_data_fn and write_data_fn in the"
          " same structure");
   }
   png_ptr->output_flush_fn = NULL;
#endif
}
