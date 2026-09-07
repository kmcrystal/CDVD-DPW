static int
process_iTXt(struct file *file)
{
   
   struct chunk *chunk = file->chunk;
   png_uint_32 length;
   png_uint_32 index = 0;
   assert(chunk != NULL && file->idat == NULL);
   length = chunk->chunk_length;
   setpos(chunk);
   while (length >= 5)
   {
      --length;
      ++index;
      if (reread_byte(file) == 0) 
      {
         --length;
         ++index;
         if (reread_byte(file) == 0) 
            return 1; 
         --length;
         ++index;
         (void)reread_byte(file); 
         
         while (length >= 9)
         {
            --length;
            ++index;
            if (reread_byte(file) == 0) 
            {
               
               while (length >= 8)
               {
                  --length;
                  ++index;
                  if (reread_byte(file) == 0) 
                     return zlib_check(file, index);
               }
            }
         }
         
         break;
      }
   }
   log_error(file, INVALID_ERROR_CODE, "iTXt chunk length");
   return 0; 
}
