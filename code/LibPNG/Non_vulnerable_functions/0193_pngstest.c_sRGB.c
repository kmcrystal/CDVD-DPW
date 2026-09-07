static png_byte
sRGB(double linear )
{
   return u8d(255 * sRGB_from_linear(linear));
}
