static void
gp_ga16(Pixel *p, png_const_voidp pb)
{
   png_const_uint_16p pp = voidcast(png_const_uint_16p, pb);
   p->r = p->g = p->b = pp[0];
   p->a = pp[1];
}
