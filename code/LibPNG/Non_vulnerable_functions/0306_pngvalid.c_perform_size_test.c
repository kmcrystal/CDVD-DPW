static void
perform_size_test(png_modifier *pm)
{
   


   if (!test_size(pm, 0, 0, READ_BDHI))
      return;
   if (!test_size(pm, 2, 3, READ_BDHI))
      return;
   


#if 0
   if (!test_size(pm, 3, 0, 3))
      return;
#endif
   if (!test_size(pm, 4, 3, READ_BDHI))
      return;
   if (!test_size(pm, 6, 3, READ_BDHI))
      return;
}
