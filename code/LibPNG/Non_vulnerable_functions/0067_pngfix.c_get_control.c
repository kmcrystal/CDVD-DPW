static struct file *
get_control(png_const_structrp png_ptr)
{
   


   struct control *control = png_voidcast(struct control*,
      png_get_error_ptr(png_ptr));
   return &control->file;
}
