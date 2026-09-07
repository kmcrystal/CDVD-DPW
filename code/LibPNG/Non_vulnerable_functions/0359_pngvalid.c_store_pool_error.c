static void
store_pool_error(png_store *ps, png_const_structp pp, PNG_CONST char *msg)
{
   if (pp != NULL)
      png_error(pp, msg);
   



   store_log(ps, pp, msg, 1 );
}
