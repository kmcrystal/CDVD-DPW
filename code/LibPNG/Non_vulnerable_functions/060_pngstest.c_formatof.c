static png_uint_32
formatof(const char *arg)
{
   char *ep;
   unsigned long format = strtoul(arg, &ep, 0);
   if (ep > arg && *ep == 0 && format < FORMAT_COUNT)
      return (png_uint_32)format;
   else for (format=0; format < FORMAT_COUNT; ++format)
   {
      if (strcmp(format_names[format], arg) == 0)
         return (png_uint_32)format;
   }
   fprintf(stderr, "pngstest: format name '%s' invalid\n", arg);
   return FORMAT_COUNT;
}
