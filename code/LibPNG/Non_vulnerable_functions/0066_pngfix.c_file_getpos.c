static void
file_getpos(struct file *file, fpos_t *pos)
{
   if (fgetpos(file->file, pos))
   {
      
      perror(file->file_name);
      stop(file, READ_ERROR_CODE, "fgetpos");
   }
}
