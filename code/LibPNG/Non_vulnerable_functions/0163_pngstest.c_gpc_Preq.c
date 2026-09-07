static void
gpc_preq(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   out->r = ilineara(in->r, in->a);
   if (in->g == in->r)
   {
      out->g = out->r;
      if (in->b == in->r)
         out->b = out->r;
      else
         out->b = ilineara(in->b, in->a);
   }
   else
   {
      out->g = ilineara(in->g, in->a);
      if (in->b == in->r)
         out->b = out->r;
      else if (in->b == in->g)
         out->b = out->g;
      else
         out->b = ilineara(in->b, in->a);
   }
   out->a = 65535;
}
