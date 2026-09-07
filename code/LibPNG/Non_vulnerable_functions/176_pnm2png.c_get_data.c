 */
png_uint_32 get_data (FILE *pnm_file, int depth)
{
  static int bits_left = 0;
  static int old_value = 0;
  static int mask = 0;
  int i;
  png_uint_32 ret_value;
  if (mask == 0)
    for (i = 0; i < depth; i++)
      mask = (mask >> 1) | 0x80;
  if (bits_left <= 0)
  {
    old_value = fgetc (pnm_file);
    bits_left = 8;
  }
  ret_value = old_value & mask;
  for (i = 1; i < (8 / depth); i++)
    ret_value = ret_value || (ret_value >> depth);
  old_value = (old_value << depth) & 0xFF;
  bits_left -= depth;
  return ret_value;
}
