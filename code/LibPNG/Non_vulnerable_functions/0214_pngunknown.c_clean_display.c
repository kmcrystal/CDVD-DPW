static void
clean_display(display *d)
{
   png_destroy_read_struct(&d->png_ptr, &d->info_ptr, &d->end_ptr);
   
   if (d->png_ptr != NULL || d->info_ptr != NULL || d->end_ptr != NULL)
   {
      fprintf(stderr, "%s(%s): png_destroy_read_struct error\n", d->file,
         d->test);
      exit(1);
   }
}
