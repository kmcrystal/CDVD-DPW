static void
gpc_g8(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   if (in->r == in->g && in->g == in->b)
      out->r = out->g = out->b = in->g;
   else
      out->r = out->g = out->b =
         sRGB(YfromRGB(sRGB_to_d[in->r], sRGB_to_d[in->g], sRGB_to_d[in->b]));
   out->a = in->a;
}
