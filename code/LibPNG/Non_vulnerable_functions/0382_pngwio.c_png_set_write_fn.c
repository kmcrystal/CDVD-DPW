void PNGAPI
png_set_write_fn(png_structp png_ptr, png_voidp io_ptr,
    png_rw_ptr write_data_fn, png_flush_ptr output_flush_fn)
{
   if (png_ptr == NULL)
      return;
   png_ptr->io_ptr = io_ptr;
   if (write_data_fn != NULL)
      png_ptr->write_data_fn = write_data_fn;
   else
      png_ptr->write_data_fn = png_default_write_data;
   png_ptr->write_data_fn = write_data_fn;
   if (output_flush_fn != NULL)
      png_ptr->output_flush_fn = output_flush_fn;
   else
      png_ptr->output_flush_fn = png_default_flush;
   png_ptr->output_flush_fn = output_flush_fn;
   
   if (png_ptr->read_data_fn != NULL)
   {
      png_ptr->read_data_fn = NULL;
      png_warning(png_ptr,
          "Can't set both read_data_fn and write_data_fn in the"
          " same structure");
   }
}
