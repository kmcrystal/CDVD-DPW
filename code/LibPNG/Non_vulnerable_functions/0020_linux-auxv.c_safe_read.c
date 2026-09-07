static size_t
safe_read(png_structp png_ptr, int fd, void *buffer_in, size_t nbytes)
{
   size_t ntotal = 0;
   char *buffer = png_voidcast(char*, buffer_in);
   while (nbytes > 0)
   {
      unsigned int nread;
      int iread;
      



      if (nbytes > INT_MAX)
         nread = INT_MAX;
      else
         nread = (unsigned int)nbytes;
      iread = read(fd, buffer, nread);
      if (iread == -1)
      {
         



         if (errno != EINTR)
         {
            png_warning(png_ptr, "/proc read failed");
            return 0; 
         }
      }
      else if (iread < 0)
      {
         
         png_warning(png_ptr, "OS /proc read bug");
         return 0;
      }
      else if (iread > 0)
      {
         
         buffer += iread;
         nbytes -= (unsigned int)iread;
         ntotal += (unsigned int)iread;
      }
      else
         return ntotal;
   }
   return ntotal; 
}
