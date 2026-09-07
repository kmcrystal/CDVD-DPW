png_voidp
PNGCBAPI png_debug_malloc(png_structp png_ptr, png_alloc_size_t size)
{
   


   if (size == 0)
      return (NULL);
   

   {
      
      memory_infop pinfo;
      png_set_mem_fn(png_ptr, NULL, NULL, NULL);
      pinfo = (memory_infop)png_malloc(png_ptr,
         png_sizeof(*pinfo));
      pinfo->size = size;
      current_allocation += size;
      total_allocation += size;
      num_allocations ++;
      if (current_allocation > maximum_allocation)
         maximum_allocation = current_allocation;
      pinfo->pointer = png_malloc(png_ptr, size);
      
      png_set_mem_fn(png_ptr,
          NULL, png_debug_malloc, png_debug_free);
      if (size != 0 && pinfo->pointer == NULL)
      {
         current_allocation -= size;
         total_allocation -= size;
         png_error(png_ptr,
           "out of memory in pngtest->png_debug_malloc");
      }
      pinfo->next = pinformation;
      pinformation = pinfo;
      
      png_memset(pinfo->pointer, 0xdd, pinfo->size);
      if (verbose)
         printf("png_malloc %lu bytes at %p\n", (unsigned long)size,
            pinfo->pointer);
      return (png_voidp)(pinfo->pointer);
   }
}
