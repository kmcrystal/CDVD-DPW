static int
read_byte(struct file *file)
{
   int ch = getc(file->file);
   if (ch >= 0 && ch <= 255)
   {
      ++(file->read_count);
      return ch;
   }
   else if (ch != EOF)
   {
      file->status_code |= INTERNAL_ERROR;
      file->read_errno = ERANGE; 
      
      emit_error(file, UNEXPECTED_ERROR_CODE, "file read");
   }
#  ifdef EINTR
      else if (errno == EINTR) 
      {
         errno = 0;
         return read_byte(file);
      }
#  endif
   else
   {
      


      if (ferror(file->file))
         file->read_errno = errno;
      else if (feof(file->file))
         file->read_errno = 0; 
      else 
         file->read_errno = EDOM;
   }
   



   file->status_code |= TRUNCATED;
   return EOF;
}
