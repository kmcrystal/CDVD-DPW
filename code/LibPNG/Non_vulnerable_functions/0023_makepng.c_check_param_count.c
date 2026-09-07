static void
check_param_count(int nparams, int expect)
{
   if (nparams != expect)
   {
      fprintf(stderr, "bad parameter count (internal error)\n");
      exit(1);
   }
}
