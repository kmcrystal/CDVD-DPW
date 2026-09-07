int
main(void)
{
   fprintf(stderr,
      "pngfix needs libpng with a zlib >=1.2.4 (not 0x%x)\n",
      ZLIB_VERNUM);
   return 77;
}
int
main(void)
{
   fprintf(stderr, "pngfix does not work without read support\n");
   return 77;
}
int
main(void)
{
   fprintf(stderr, "pngfix does not work without setjmp support\n");
   return 77;
}
