int _TIFF_vsnprintf_f(char* str, size_t size, const char* format, va_list ap)
{
  int count = -1;
  if (size != 0)
    count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
  if (count == -1)
    count = _vscprintf(format, ap);
  return count;
}
