static int
logclose(Image *image, FILE *f, const char *name, const char *operation)
{
   int e = errno;
   fclose(f);
   return logerror(image, name, operation, strerror(e));
}
