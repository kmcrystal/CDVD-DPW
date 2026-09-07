}
static int format_set(format_list *pf, png_uint_32 format)
{
   if (format < FORMAT_COUNT)
      return pf->bits[format >> 5] |= ((png_uint_32)1) << (format & 31);
   return 0;
}
