#else 
int main(void)
{
   fprintf(stderr,
      "pngvalid: no low level write support in libpng, all tests skipped\n");
   
   return SKIP;
}
