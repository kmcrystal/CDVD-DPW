static void
gpc_cb16(Pixel *out, const Pixel *in, const Background *back)
{
   if (in->a <= 0)
   {
      out->r = back->ir;
      out->g = back->ig;
      out->b = back->ib;
   }
   else if (in->a >= 65535)
   {
      out->r = isRGB(in->r);
      out->g = isRGB(in->g);
      out->b = isRGB(in->b);
   }
   else
   {
      double a = in->a / 65535.;
      double a1 = 1-a;
      a /= 65535;
      out->r = sRGB(in->r * a + back->dr * a1);
      out->g = sRGB(in->g * a + back->dg * a1);
      out->b = sRGB(in->b * a + back->db * a1);
   }
   out->a = 255;
}
