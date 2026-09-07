void PNGCBAPI
png_debug_free(png_structp png_ptr, png_voidp ptr)
{
   if (png_ptr == NULL)
      fprintf(STDERR, "NULL pointer to png_debug_free.\n");
   if (ptr == 0)
   {
#if 0 
      fprintf(STDERR, "WARNING: freeing NULL pointer\n");
#endif
      return;
   }
   
   {
      memory_infop FAR *ppinfo = &pinformation;
      for (;;)
      {
         memory_infop pinfo = *ppinfo;
         if (pinfo->pointer == ptr)
         {
            *ppinfo = pinfo->next;
            current_allocation -= pinfo->size;
            if (current_allocation < 0)
               fprintf(STDERR, "Duplicate free of memory\n");
            

            png_memset(ptr, 0x55, pinfo->size);
            png_free_default(png_ptr, pinfo);
            pinfo = NULL;
            break;
         }
         if (pinfo->next == NULL)
         {
            fprintf(STDERR, "Pointer %x not found\n", (unsigned int)ptr);
            break;
         }
         ppinfo = &pinfo->next;
      }
   }
   
   if (verbose)
      printf("Freeing %p\n", ptr);
   png_free_default(png_ptr, ptr);
   ptr = NULL;
}
