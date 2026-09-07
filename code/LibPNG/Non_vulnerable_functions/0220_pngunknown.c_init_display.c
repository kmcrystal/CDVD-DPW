static void
init_display(display *d, const char *program)
{
   memset(d, 0, sizeof *d);
   d->png_ptr = NULL;
   d->info_ptr = d->end_ptr = NULL;
   d->error_count = d->warning_count = 0;
   d->program = program;
   d->file = program;
   d->test = init;
}
