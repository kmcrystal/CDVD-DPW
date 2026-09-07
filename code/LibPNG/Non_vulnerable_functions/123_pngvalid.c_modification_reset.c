static void
modification_reset(png_modification *pmm)
{
   if (pmm != NULL)
   {
      pmm->modified = 0;
      pmm->added = 0;
      pmm->removed = 0;
      modification_reset(pmm->next);
   }
}
