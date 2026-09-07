#else 
int main(void)
{
   fprintf(stderr, "pngstest: no read support in libpng, test skipped\n");
   
   return 77;
}
