static const char *
strcode(int code)
{
   switch (code)
   {
      case LIBPNG_WARNING_CODE:   return "warning";
      case LIBPNG_ERROR_CODE:     return "libpng";
      case ZLIB_ERROR_CODE:       return "zlib";
      case INVALID_ERROR_CODE:    return "invalid";
      case READ_ERROR_CODE:       return "read";
      case WRITE_ERROR_CODE:      return "write";
      case UNEXPECTED_ERROR_CODE: return "unexpected";
      default:                    return "INVALID";
   }
}
