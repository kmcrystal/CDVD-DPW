static void
gp_argb8(Pixel *p, png_const_voidp pb)
{
   png_const_bytep pp = voidcast(png_const_bytep, pb);
   p->r = pp[1];
   p->g = pp[2];
   p->b = pp[3];
   p->a = pp[0];
}
