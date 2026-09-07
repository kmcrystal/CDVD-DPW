static void
gpc_gb16(Pixel *out, const Pixel *in, const Background *back)
{
   if (in->a <= 0)
      out->r = out->g = out->b = back->ig;
   else if (in->a >= 65535)
      out->r = out->g = out->b = isRGB(in->g);
   else
   {
      double a = in->a / 65535.;
      double a1 = 1-a;
      a /= 65535;
      out->r = out->g = out->b = sRGB(in->g * a + back->dg * a1);
   }
   out->a = 255;
}
