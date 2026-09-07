static void
gp_rgb8(Pixel *p, png_const_voidp pb)
{
   png_const_bytep pp = voidcast(png_const_bytep, pb);
   p->r = pp[0];
   p->g = pp[1];
   p->b = pp[2];
   p->a = 255;
}
