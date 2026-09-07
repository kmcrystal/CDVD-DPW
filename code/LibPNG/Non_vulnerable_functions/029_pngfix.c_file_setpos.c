static void
file_setpos(struct file *file, const fpos_t *pos)
{
   if (fsetpos(file->file, pos))
   {
      perror(file->file_name);
      stop(file, READ_ERROR_CODE, "fsetpos");
   }
}
