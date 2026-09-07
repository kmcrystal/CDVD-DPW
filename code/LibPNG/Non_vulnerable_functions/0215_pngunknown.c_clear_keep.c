static void
clear_keep(void)
{
   int i = NINFO;
   while (--i >= 0)
      chunk_info[i].keep = 0;
}
