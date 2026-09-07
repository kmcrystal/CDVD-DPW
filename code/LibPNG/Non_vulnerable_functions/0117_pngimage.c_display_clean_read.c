static void
display_clean_read(struct display *dp)
{
   if (dp->read_pp != NULL)
      png_destroy_read_struct(&dp->read_pp, &dp->read_ip, NULL);
}
