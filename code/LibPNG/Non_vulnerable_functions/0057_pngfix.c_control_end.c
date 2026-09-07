static int
control_end(struct control *control)
{
   return file_end(&control->file);
}
