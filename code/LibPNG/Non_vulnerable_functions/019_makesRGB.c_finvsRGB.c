double
finvsRGB(unsigned int i)
{
   return 65535 * linear_from_sRGB(i/255.);
}
