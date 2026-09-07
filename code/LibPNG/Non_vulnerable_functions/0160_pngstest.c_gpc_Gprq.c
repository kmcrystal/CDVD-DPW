static void
gpc_gprq(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   if (in->r == in->g && in->g == in->b)
      out->r = out->g = out->b = ilineara(in->g, in->a);
   else
      out->r = out->g = out->b = u16d(in->a * 257 *
         YfromRGB(sRGB_to_d[in->r], sRGB_to_d[in->g], sRGB_to_d[in->b]));
   out->a = 65535;
}
