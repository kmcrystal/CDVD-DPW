static png_uint_16
ilineara(int fixed_srgb, int alpha)
{
   return u16d((257 * alpha) * sRGB_to_d[fixed_srgb]);
}
