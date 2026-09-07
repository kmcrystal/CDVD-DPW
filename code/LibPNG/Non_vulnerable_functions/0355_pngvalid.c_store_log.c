static void
store_log(png_store* ps, png_const_structp pp, png_const_charp message,
   int is_error)
{
   



   if (is_error ? (ps->nerrors)++ == 0 :
       (ps->nwarnings)++ == 0 && ps->nerrors == 0)
      store_message(ps, pp, ps->error, sizeof ps->error, 0, message);
   if (ps->verbose)
      store_verbose(ps, pp, is_error ? "error: " : "warning: ", message);
}
