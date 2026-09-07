}
int main(int argc, char **argv)
{
   int ok = 0;
   FILE *fp = tmpfile();
   if (fp != NULL)
   {
      int err = 0;
      int nfiles = 0;
      if (argc > 1)
      {
         int i;
         for (i=1; i<argc; ++i)
         {
            if (add_one_file(fp, argv[i]))
               ++nfiles;
            else
            {
               err = 1;
               break;
            }
         }
      }
      else
      {
         char filename[FILENAME_MAX+1];
         while (fgets(filename, FILENAME_MAX+1, stdin))
         {
            size_t len = strlen(filename);
            if (filename[len-1] == '\n')
            {
               filename[len-1] = 0;
               if (add_one_file(fp, filename))
                  ++nfiles;
               else
               {
                  err = 1;
                  break;
               }
            }
            else
            {
               fprintf(stderr, "timepng: truncated file name ...%s\n",
                  filename+len-32);
               err = 1;
               break;
            }
         }
         if (ferror(stdin))
         {
            fprintf(stderr, "timepng: stdin: read error\n");
            err = 1;
         }
      }
      if (!err)
      {
         if (nfiles > 0)
            ok = perform_one_test(fp, nfiles);
         else
            fprintf(stderr, "usage: timepng {files} or ls files | timepng\n");
      }
      (void)fclose(fp);
   }
   else
      fprintf(stderr, "timepng: could not open temporary file\n");
   
   return ok == 0;
}
