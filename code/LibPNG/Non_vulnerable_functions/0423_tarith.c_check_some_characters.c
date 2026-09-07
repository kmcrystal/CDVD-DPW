}
static int check_some_characters(checkfp_command *co, checkfp_control c,
   const char *tests)
{
   int i;
   --(c.limit);
   if (c.cnumber+4 < sizeof co->number && c.limit >= 0)
   {
      if (c.limit > 0) for (i=0; tests[i]; ++i)
      {
         if (!check_one_character(co, c, tests[i]))
               return 0;
      }
      
      else
         return check_all_characters(co, c);
   }
   return 1;
}
