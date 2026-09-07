static void
gpc_nop6(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   if (in->a == 0)
      out->r = out->g = out->b = 65535;
   else
   {
      out->r = in->r;
      out->g = in->g;
      out->b = in->b;
   }
   out->a = in->a;
}
