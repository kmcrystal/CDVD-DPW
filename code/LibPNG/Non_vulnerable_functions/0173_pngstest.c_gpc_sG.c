static void
gpc_sG(Pixel *out, const Pixel *in, const Background *back)
{
   (void)back;
   out->r = out->g = out->b = sRGB(YfromRGBint(in->r, in->g, in->b)/65535);
   out->a = 255;
}
