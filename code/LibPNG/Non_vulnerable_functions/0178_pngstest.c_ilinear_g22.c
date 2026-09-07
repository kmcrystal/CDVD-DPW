static png_uint_16
ilinear_g22(int fixed_srgb)
{
   return u16d(65535 * g22_to_d[fixed_srgb]);
}
