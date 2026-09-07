static int
srgb_modify(png_modifier *pm, png_modification *me, int add)
{
   UNUSED(add)
   
   png_save_uint_32(pm->buffer, 1);
   png_save_uint_32(pm->buffer+4, CHUNK_sRGB);
   pm->buffer[8] = ((srgb_modification*)me)->intent;
   return 1;
}
