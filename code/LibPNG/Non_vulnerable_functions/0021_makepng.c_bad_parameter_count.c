static void
bad_parameter_count(png_const_charp what, int nparams)
{
   fprintf(stderr, "--insert %s: bad parameter count %d\n", what, nparams);
   exit(1);
}
