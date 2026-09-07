static png_uint_16
ilineara_g22(int fixed_srgb, int alpha)
{
   return u16d((257 * alpha) * g22_to_d[fixed_srgb]);
}
