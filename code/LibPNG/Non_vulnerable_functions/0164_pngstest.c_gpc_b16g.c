static void
gpc_b16g(Pixel *out, const Pixel *in, const Background *back)
{
   if (in->a <= 0)
      out->r = out->g = out->b = back->ig;
   else
   {
      double a = in->a/65535.;
      double a1 = 1-a;
      a /= 65535;
      out->r = out->g = out->b = sRGB(in->g * a + back->dg * a1);
   }
   out->a = 255;
}
