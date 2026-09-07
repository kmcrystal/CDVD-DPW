static void
gp_ag16(Pixel *p, png_const_voidp pb)
{
   png_const_uint_16p pp = voidcast(png_const_uint_16p, pb);
   p->r = p->g = p->b = pp[1];
   p->a = pp[0];
}
