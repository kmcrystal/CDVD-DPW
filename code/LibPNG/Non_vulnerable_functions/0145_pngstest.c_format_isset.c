#endif
static int format_isset(format_list *pf, png_uint_32 format)
{
   return format < FORMAT_COUNT &&
      (pf->bits[format >> 5] & (((png_uint_32)1) << (format & 31))) != 0;
}
