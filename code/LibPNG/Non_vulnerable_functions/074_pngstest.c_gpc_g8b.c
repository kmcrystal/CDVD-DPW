static void
gpc_g8b(Pixel *out, const Pixel *in, const Background *back)
{
   if (in->a <= 0)
      out->r = out->g = out->b = back->ig;
   else if (in->a >= 255)
   {
      if (in->r == in->g && in->g == in->b)
         out->r = out->g = out->b = in->g;
      else
         out->r = out->g = out->b = sRGB(YfromRGB(
            sRGB_to_d[in->r], sRGB_to_d[in->g], sRGB_to_d[in->b]));
   }
   else
   {
      double a = in->a/255.;
      out->r = out->g = out->b = sRGB(a * YfromRGB(sRGB_to_d[in->r],
         sRGB_to_d[in->g], sRGB_to_d[in->b]) + back->dg * (1-a));
   }
   out->a = 255;
}
