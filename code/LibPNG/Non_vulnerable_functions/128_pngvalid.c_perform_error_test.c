static void
perform_error_test(png_modifier *pm)
{
#ifdef PNG_WARNINGS_SUPPORTED 
   
   safecat(pm->this.test, sizeof pm->this.test, 0, "error test");
   if (!make_errors(pm, 0, 0, WRITE_BDHI))
      return;
   if (!make_errors(pm, 2, 3, WRITE_BDHI))
      return;
   if (!make_errors(pm, 3, 0, 3))
      return;
   if (!make_errors(pm, 4, 3, WRITE_BDHI))
      return;
   if (!make_errors(pm, 6, 3, WRITE_BDHI))
      return;
#else
   UNUSED(pm)
#endif
}
