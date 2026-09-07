}
static size_t safecatn(char *buffer, size_t bufsize, size_t pos, int n)
{
   char number[64];
   sprintf(number, "%d", n);
   return safecat(buffer, bufsize, pos, number);
}
