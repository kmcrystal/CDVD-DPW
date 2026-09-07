static void
gpc_A(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   out->r = in->r;
   out->g = in->g;
   out->b = in->b;
   out->a = 65535;
}
