#ifdef PNG_READ_TRANSFORMS_SUPPORTED
static size_t safecatd(char *buffer, size_t bufsize, size_t pos, double d,
    int precision)
{
   char number[64];
   sprintf(number, "%.*f", precision, d);
   return safecat(buffer, bufsize, pos, number);
}
