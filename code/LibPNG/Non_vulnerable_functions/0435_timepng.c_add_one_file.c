}
static int add_one_file(FILE *fp, char *name)
{
   FILE *ip = fopen(name, "rb");
   if (ip != NULL)
   {
      int ch;
      for (;;)
      {
         ch = getc(ip);
         if (ch == EOF) break;
         putc(ch, fp);
      }
      if (ferror(ip))
      {
         perror(name);
         fprintf(stderr, "%s: read error\n", name);
         return 0;
      }
      (void)fclose(ip);
      if (ferror(fp))
      {
         perror("temporary file");
         fprintf(stderr, "temporary file write error\n");
         return 0;
      }
   }
   else
   {
      perror(name);
      fprintf(stderr, "%s: open failed\n", name);
      return 0;
   }
   return 1;
}
