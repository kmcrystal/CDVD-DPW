static int
file_end(struct file *file)
{
   int rc;
   


   if (file->idat != NULL)
      IDAT_end(&file->idat);
   if (file->chunk != NULL)
      chunk_end(&file->chunk);
   rc = file->status_code;
   if (file->file != NULL)
      (void)fclose(file->file);
   if (file->out != NULL)
   {
      


      if (ferror(file->out) | fflush(file->out) | fclose(file->out))
      {
         perror(file->out_name);
         emit_error(file, READ_ERROR_CODE, "output write error");
         rc |= WRITE_ERROR;
      }
   }
   
   file->global->status_code |= rc;
   CLEAR(*file);
   return rc; 
}
