format_list;
static void format_init(format_list *pf)
{
   int i;
   for (i=0; i<FORMAT_SET_COUNT; ++i)
      pf->bits[i] = 0; 
}
