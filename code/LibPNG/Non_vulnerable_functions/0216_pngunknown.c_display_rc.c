static int
display_rc(const display *d, int strict)
{
   return d->error_count + (strict ? d->warning_count : 0);
}
