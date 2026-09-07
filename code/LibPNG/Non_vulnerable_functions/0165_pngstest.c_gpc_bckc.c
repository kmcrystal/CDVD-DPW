static void
gpc_bckc(Pixel *out, const Pixel *in, const Background *back)
{
   if (in->a <= 0)
   {
      out->r = back->ir;
      out->g = back->ig;
      out->b = back->ib;
   }
   else if (in->a >= 255)
   {
      out->r = in->r;
      out->g = in->g;
      out->b = in->b;
   }
   else
   {
      double a = in->a / 255.;
      out->r = sRGB(sRGB_to_d[in->r] * a + back->dr * (1-a));
      out->g = sRGB(sRGB_to_d[in->g] * a + back->dg * (1-a));
      out->b = sRGB(sRGB_to_d[in->b] * a + back->db * (1-a));
   }
   out->a = 255;
}
