static void
store_freefile(png_store_file **ppf)
{
   if (*ppf != NULL)
   {
      store_freefile(&(*ppf)->next);
      store_freebuffer(&(*ppf)->data);
      (*ppf)->datacount = 0;
      if ((*ppf)->palette != NULL)
      {
         free((*ppf)->palette);
         (*ppf)->palette = NULL;
         (*ppf)->npalette = 0;
      }
      free(*ppf);
      *ppf = NULL;
   }
}
