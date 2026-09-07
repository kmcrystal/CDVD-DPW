static int
random_choice(void)
{
   unsigned char x;
   RANDOMIZE(x);
   return x & 1;
}
