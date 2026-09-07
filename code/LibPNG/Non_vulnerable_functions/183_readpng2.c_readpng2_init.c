
int readpng2_init(mainprog_info *mainprog_ptr)
{
    png_structp  png_ptr;       
    png_infop  info_ptr;
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, mainprog_ptr,
      readpng2_error_handler, NULL);
    if (!png_ptr)
        return 4;   
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 4;   
    }
    


    



    if (setjmp(mainprog_ptr->jmpbuf)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 2;
    }
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
    


    {
        


        static PNG_CONST png_byte chunks_to_process[] = {
            98,  75,  71,  68, '\0',  
           103,  65,  77,  65, '\0',  
           115,  82,  71,  66, '\0',  
           };
       
       png_set_keep_unknown_chunks(png_ptr, -1 ,
          NULL, -1);
       
       png_set_keep_unknown_chunks(png_ptr,
          0 , chunks_to_process,
          sizeof(chunks_to_process)/5);
    }
#endif 
    

    png_set_progressive_read_fn(png_ptr, mainprog_ptr,
      readpng2_info_callback, readpng2_row_callback, readpng2_end_callback);
    
    mainprog_ptr->png_ptr = png_ptr;
    mainprog_ptr->info_ptr = info_ptr;
    
    return 0;
}
