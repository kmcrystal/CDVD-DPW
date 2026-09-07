static int
get_page_geometry (char *name, struct pagedef *page)
    {
    char *ptr;
    int n; 
    for (ptr = name; *ptr; ptr++)
      *ptr = (char)tolower((int)*ptr);
    for (n = 0; n < MAX_PAPERNAMES; n++)
      {
      if (strcmp(name, PaperTable[n].name) == 0)
        {
	page->width = PaperTable[n].width;
	page->length = PaperTable[n].length;
        strncpy (page->name, PaperTable[n].name, 15);
        page->name[15] = '\0';
        return (0);
        }
      }
  return (1);
  }
