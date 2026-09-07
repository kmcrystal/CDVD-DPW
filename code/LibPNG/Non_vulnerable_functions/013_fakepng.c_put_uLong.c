void
put_uLong(uLong val)
{
   putchar(val >> 24);
   putchar(val >> 16);
   putchar(val >>  8);
   putchar(val >>  0);
}
