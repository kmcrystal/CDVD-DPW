static void
gp_abgr16(Pixel *p, png_const_voidp pb)
{
   png_const_uint_16p pp = voidcast(png_const_uint_16p, pb);
   p->r = pp[3];
   p->g = pp[2];
   p->b = pp[1];
   p->a = pp[0];
}
