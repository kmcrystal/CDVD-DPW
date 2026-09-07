double
fsRGB(double l)
{
   return sRGB_from_linear(l/max_input);
}
