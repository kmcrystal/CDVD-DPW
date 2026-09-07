static const char *
zlib_flevel(struct zlib *zlib)
{
   switch (zlib->header[1] >> 6)
   {
      case 0:  return "supfast";
      case 1:  return "stdfast";
      case 2:  return "default";
      case 3:  return "maximum";
      default: assert(UNREACHED);
   }
   return "COMPILER BUG";
}
