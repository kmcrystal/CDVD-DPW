     */
static int PNGCBAPI read_user_chunk_callback(png_struct *png_ptr,
   png_unknown_chunkp chunk)
{
   png_uint_32
     *my_user_chunk_data;
   











   if (chunk->name[0] == 115 && chunk->name[1] ==  84 &&     
       chunk->name[2] ==  69 && chunk->name[3] ==  82)       
      {
         
         if (chunk->size != 1)
            return (-1); 
         if (chunk->data[0] != 0 && chunk->data[0] != 1)
            return (-1);  
         my_user_chunk_data=(png_uint_32 *) png_get_user_chunk_ptr(png_ptr);
         my_user_chunk_data[0]=chunk->data[0]+1;
         return (1);
      }
   if (chunk->name[0] != 118 || chunk->name[1] != 112 ||    
       chunk->name[2] !=  65 || chunk->name[3] != 103)      
      return (0); 
   
   if (chunk->size != 9)
      return (-1); 
   my_user_chunk_data=(png_uint_32 *) png_get_user_chunk_ptr(png_ptr);
   my_user_chunk_data[1]=png_get_uint_31(png_ptr, chunk->data);
   my_user_chunk_data[2]=png_get_uint_31(png_ptr, chunk->data + 4);
   my_user_chunk_data[3]=(png_uint_32)chunk->data[8];
   return (1);
}
