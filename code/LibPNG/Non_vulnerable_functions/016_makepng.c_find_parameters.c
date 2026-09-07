static int
find_parameters(png_const_charp what, png_charp param, png_charp *list,
   int nparams)
{
   


   int i;
   for (i=0; *param && i<nparams; ++i)
   {
      list[i] = param;
      while (*++param) if (*param == '\n' || *param == ':')
      {
         *param++ = 0; 
         break;        
      }
   }
   if (*param)
   {
      fprintf(stderr, "--insert %s: too many parameters (%s)\n", what, param);
      exit(1);
   }
   list[i] = NULL; 
   return i; 
}
