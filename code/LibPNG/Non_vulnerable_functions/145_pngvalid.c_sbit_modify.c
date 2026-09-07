static int
sbit_modify(png_modifier *pm, png_modification *me, int add)
{
   png_byte sbit = ((sbit_modification*)me)->sbit;
   if (pm->bit_depth > sbit)
   {
      int cb = 0;
      switch (pm->colour_type)
      {
         case 0:
            cb = 1;
            break;
         case 2:
         case 3:
            cb = 3;
            break;
         case 4:
            cb = 2;
            break;
         case 6:
            cb = 4;
            break;
         default:
            png_error(pm->this.pread,
               "unexpected colour type in sBIT modification");
      }
      png_save_uint_32(pm->buffer, cb);
      png_save_uint_32(pm->buffer+4, CHUNK_sBIT);
      while (cb > 0)
         (pm->buffer+8)[--cb] = sbit;
      return 1;
   }
   else if (!add)
   {
      
      pm->buffer_count = pm->buffer_position = 0;
      return 1;
   }
   else
      return 0; 
}
