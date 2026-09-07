static void
gpc_b16c(Pixel *out, const Pixel *in, const Background *back)
{
   if (in->a <= 0)
   {
      out->r = back->ir;
      out->g = back->ig;
      out->b = back->ib;
   }
   else
   {
      double a = in->a/65535.;
      double a1 = 1-a;
      a /= 65535;
      out->r = sRGB(in->r * a + back->dr * a1);
      out->g = sRGB(in->g * a + back->dg * a1);
      out->b = sRGB(in->b * a + back->db * a1);
   }
   out->a = 255;
}
