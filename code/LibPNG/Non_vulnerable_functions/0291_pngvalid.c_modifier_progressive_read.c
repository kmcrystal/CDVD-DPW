static void
modifier_progressive_read(png_modifier *pm, png_structp pp, png_infop pi)
{
   if (pm->this.pread != pp || pm->this.current == NULL ||
       pm->this.next == NULL)
      png_error(pp, "store state damaged (progressive)");
   





   for (;;)
   {
      static png_uint_32 noise = 1;
      png_size_t cb, cbAvail;
      png_byte buffer[512];
      
      noise = (noise << 9) | ((noise ^ (noise >> (9-5))) & 0x1ff);
      cb = noise & 0x1ff;
      




      cbAvail = store_read_buffer_avail(&pm->this);
      if (pm->buffer_count > pm->buffer_position)
         cbAvail += pm->buffer_count - pm->buffer_position;
      if (cb > cbAvail)
      {
         
         if (cbAvail == 0)
            break;
         cb = cbAvail;
      }
      modifier_read_imp(pm, buffer, cb);
      png_process_data(pp, pi, buffer, cb);
   }
   


   if (pm->buffer_count > pm->buffer_position ||
       pm->this.next != &pm->this.current->data ||
       pm->this.readpos < pm->this.current->datacount)
      png_error(pp, "progressive read implementation error");
}
