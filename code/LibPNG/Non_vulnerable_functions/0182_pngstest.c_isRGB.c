static png_byte
isRGB(int fixed_linear)
{
   return sRGB(fixed_linear / 65535.);
}
