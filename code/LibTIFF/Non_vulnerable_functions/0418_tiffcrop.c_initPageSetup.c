static void
initPageSetup (struct pagedef *page, struct pageseg *pagelist, 
               struct buffinfo seg_buffs[])
   {
   int i; 
   strcpy (page->name, "");
   page->mode = PAGE_MODE_NONE;
   page->res_unit = RESUNIT_NONE;
   page->hres = 0.0;
   page->vres = 0.0;
   page->width = 0.0;
   page->length = 0.0;
   page->hmargin = 0.0;
   page->vmargin = 0.0;
   page->rows = 0;
   page->cols = 0;
   page->orient = ORIENTATION_NONE;
   for (i = 0; i < MAX_SECTIONS; i++)
     {
     pagelist[i].x1 = (uint32)0;
     pagelist[i].x2 = (uint32)0;
     pagelist[i].y1 = (uint32)0;
     pagelist[i].y2 = (uint32)0;
     pagelist[i].buffsize = (uint32)0;
     pagelist[i].position = 0;
     pagelist[i].total = 0;
     }
   for (i = 0; i < MAX_OUTBUFFS; i++)
     {
     seg_buffs[i].size = 0;
     seg_buffs[i].buffer = NULL;
     }
   }
