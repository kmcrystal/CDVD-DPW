static void
store_storefile(png_store *ps, png_uint_32 id)
{
   png_store_file *pf = voidcast(png_store_file*, malloc(sizeof *pf));
   if (pf == NULL)
      png_error(ps->pwrite, "storefile: OOM");
   safecat(pf->name, sizeof pf->name, 0, ps->wname);
   pf->id = id;
   pf->data = ps->new;
   pf->datacount = ps->writepos;
   ps->new.prev = NULL;
   ps->writepos = 0;
   pf->palette = ps->palette;
   pf->npalette = ps->npalette;
   ps->palette = 0;
   ps->npalette = 0;
   
   pf->next = ps->saved;
   ps->saved = pf;
}
