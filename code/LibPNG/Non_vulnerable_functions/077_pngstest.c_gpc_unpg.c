static void
gpc_unpg(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   if (in->a <= 128)
   {
      out->r = out->g = out->b = 255;
      out->a = 0;
   }
   else
   {
      out->r = out->g = out->b = sRGB((double)in->g / in->a);
      out->a = u8d(in->a / 257.);
   }
}
