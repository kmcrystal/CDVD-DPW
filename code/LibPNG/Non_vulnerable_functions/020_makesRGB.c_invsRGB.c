double
finvsRGB(unsigned int i)
{
   return 65535 * linear_from_sRGB(i/255.);
}
png_uint_16
invsRGB(unsigned int i)
{
   unsigned int x = nearbyint(finvsRGB(i));
   if (x > 65535)
   {
      fprintf(stderr, "invsRGB(%u) overflows to %u\n", i, x);
      exit(1);
   }
   return (png_uint_16)x;
}
