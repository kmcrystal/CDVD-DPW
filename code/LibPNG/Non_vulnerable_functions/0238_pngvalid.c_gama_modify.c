static int
gama_modify(png_modifier *pm, png_modification *me, int add)
{
   UNUSED(add)
   
   png_save_uint_32(pm->buffer, 4);
   png_save_uint_32(pm->buffer+4, CHUNK_gAMA);
   png_save_uint_32(pm->buffer+8, ((gama_modification*)me)->gamma);
   return 1;
}
