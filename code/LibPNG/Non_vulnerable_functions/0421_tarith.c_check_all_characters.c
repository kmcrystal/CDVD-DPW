}
static int check_all_characters(checkfp_command *co, checkfp_control c)
{
   int ch;
   if (c.cnumber+4 < sizeof co->number) for (ch=0; ch<256; ++ch)
   {
      if (!check_one_character(co, c, ch))
         return 0;
   }
   return 1;
}
