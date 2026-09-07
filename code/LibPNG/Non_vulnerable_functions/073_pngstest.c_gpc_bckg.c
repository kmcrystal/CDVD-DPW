static void
gpc_bckg(Pixel *out, const Pixel *in, const Background *back)
{
   if (in->a <= 0)
      out->r = out->g = out->b = back->ig;
   else if (in->a >= 255)
      out->r = out->g = out->b = in->g;
   else
   {
      double a = in->a / 255.;
      out->r = out->g = out->b = sRGB(sRGB_to_d[in->g] * a + back->dg * (1-a));
   }
   out->a = 255;
}
