unsigned long
component(const char *prog, const char *arg, int issRGB)
{
   char *ep;
   unsigned long c = strtoul(arg, &ep, 0);
   if (ep <= arg || *ep || c > 65535 || (issRGB && c > 255))
   {
      fprintf(stderr, "%s: %s: invalid component value (%lu)\n", prog, arg, c);
      usage(prog);
   }
   return c;
}
