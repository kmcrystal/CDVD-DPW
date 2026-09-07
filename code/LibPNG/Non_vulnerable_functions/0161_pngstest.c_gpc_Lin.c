static void
gpc_lin(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   out->r = ilinear(in->r);
   if (in->g == in->r)
   {
      out->g = out->r;
      if (in->b == in->r)
         out->b = out->r;
      else
         out->b = ilinear(in->b);
   }
   else
   {
      out->g = ilinear(in->g);
      if (in->b == in->r)
         out->b = out->r;
      else if (in->b == in->g)
         out->b = out->g;
      else
         out->b = ilinear(in->b);
   }
   out->a = 65535;
}
