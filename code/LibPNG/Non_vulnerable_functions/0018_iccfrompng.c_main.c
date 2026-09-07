int
main(int argc, char **argv)
{
   int i;
   int extracted = 0;
   for (i=1; i<argc; ++i)
   {
      if (strcmp(argv[i], "-q") == 0)
         verbose = 0;
      else if (extract_one_file(argv[i]))
         extracted = 1;
   }
   
   return extracted == 0;
}
