static png_uint_32
current_type(struct file *file, int code)
   
{
   




   if (file->chunk != NULL)
   {
      png_uint_32 type = file->chunk->chunk_type;
      



      if (code <= LIBPNG_ERROR_CODE && type == png_IDAT &&
         file->write_count == 8)
         type = 0; 
      return type;
   }
   else
      return file->type;
}
