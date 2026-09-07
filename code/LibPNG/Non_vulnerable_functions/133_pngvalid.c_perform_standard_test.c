static void
perform_standard_test(png_modifier *pm)
{
   


   if (!test_standard(pm, 0, 0, READ_BDHI))
      return;
   if (!test_standard(pm, 2, 3, READ_BDHI))
      return;
   if (!test_standard(pm, 3, 0, 3))
      return;
   if (!test_standard(pm, 4, 3, READ_BDHI))
      return;
   if (!test_standard(pm, 6, 3, READ_BDHI))
      return;
}
