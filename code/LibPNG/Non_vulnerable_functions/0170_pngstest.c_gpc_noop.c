static void
gpc_noop(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   out->r = in->r;
   out->g = in->g;
   out->b = in->b;
   out->a = in->a;
}
