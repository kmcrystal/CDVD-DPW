static chunk_insert *
make_insert(png_const_charp what,
   void (*insert)(png_structp, png_infop, int, png_charpp),
   int nparams, png_charpp list)
{
   int i;
   chunk_insert *cip;
   cip = malloc(offsetof(chunk_insert,parameters) +
      nparams * sizeof (png_charp));
   if (cip == NULL)
   {
      fprintf(stderr, "--insert %s: out of memory allocating %d parameters\n",
         what, nparams);
      exit(1);
   }
   cip->next = NULL;
   cip->insert = insert;
   cip->nparams = nparams;
   for (i=0; i<nparams; ++i)
      cip->parameters[i] = list[i];
   return cip;
}
