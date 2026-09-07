#endif
static int format_is_initial(format_list *pf)
{
   int i;
   for (i=0; i<FORMAT_SET_COUNT; ++i)
      if (pf->bits[i] != 0)
         return 0;
   return 1;
}
