static void
gpc_unpc(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   if (in->a <= 128)
   {
      out->r = out->g = out->b = 255;
      out->a = 0;
   }
   else
   {
      out->r = sRGB((double)in->r / in->a);
      out->g = sRGB((double)in->g / in->a);
      out->b = sRGB((double)in->b / in->a);
      out->a = u8d(in->a / 257.);
   }
}
