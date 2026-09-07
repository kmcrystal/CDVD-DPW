static void
gpc_sRGB(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   out->r = isRGB(in->r);
   if (in->g == in->r)
   {
      out->g = out->r;
      if (in->b == in->r)
         out->b = out->r;
      else
         out->b = isRGB(in->b);
   }
   else
   {
      out->g = isRGB(in->g);
      if (in->b == in->r)
         out->b = out->r;
      else if (in->b == in->g)
         out->b = out->g;
      else
         out->b = isRGB(in->b);
   }
   out->a = 255;
}
