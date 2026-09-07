static void
gp_bgra8(Pixel *p, png_const_voidp pb)
{
   png_const_bytep pp = voidcast(png_const_bytep, pb);
   p->r = pp[2];
   p->g = pp[1];
   p->b = pp[0];
   p->a = pp[3];
}
