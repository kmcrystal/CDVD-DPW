static png_uint_16
ilinear(int fixed_srgb)
{
   return u16d(65535 * sRGB_to_d[fixed_srgb]);
}
