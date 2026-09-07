static void
usage(const char *prog)
{
   fprintf(stderr,
      "%s: usage: %s [-linear|-sRGB] [-gray|-color] component{1,4}\n",
      prog, prog);
   exit(1);
}
