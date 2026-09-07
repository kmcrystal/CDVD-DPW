static int
chrm_modify(png_modifier *pm, png_modification *me, int add)
{
   UNUSED(add)
   
   png_save_uint_32(pm->buffer   , 32);
   png_save_uint_32(pm->buffer+ 4, CHUNK_cHRM);
   png_save_uint_32(pm->buffer+ 8, ((chrm_modification*)me)->wx);
   png_save_uint_32(pm->buffer+12, ((chrm_modification*)me)->wy);
   png_save_uint_32(pm->buffer+16, ((chrm_modification*)me)->rx);
   png_save_uint_32(pm->buffer+20, ((chrm_modification*)me)->ry);
   png_save_uint_32(pm->buffer+24, ((chrm_modification*)me)->gx);
   png_save_uint_32(pm->buffer+28, ((chrm_modification*)me)->gy);
   png_save_uint_32(pm->buffer+32, ((chrm_modification*)me)->bx);
   png_save_uint_32(pm->buffer+36, ((chrm_modification*)me)->by);
   return 1;
}
