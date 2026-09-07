static void
gp_bgr16(Pixel *p, png_const_voidp pb)
{
   png_const_uint_16p pp = voidcast(png_const_uint_16p, pb);
   p->r = pp[2];
   p->g = pp[1];
   p->b = pp[0];
   p->a = 65535;
}
