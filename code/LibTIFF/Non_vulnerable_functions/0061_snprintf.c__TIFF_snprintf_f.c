int _TIFF_snprintf_f(char* str, size_t size, const char* format, ...)
{
  int count;
  va_list ap;
  va_start(ap, format);
  count = vsnprintf(str, size, format, ap);
  va_end(ap);
  return count;
}
