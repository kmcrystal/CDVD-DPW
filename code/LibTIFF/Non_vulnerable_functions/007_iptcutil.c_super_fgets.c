int *next,char *quoted);
char *super_fgets(char *b, int *blen, FILE *file)
{
  int
    c,
    len;
  char
    *q;
  len=*blen;
  for (q=b; ; q++)
    {
      c=fgetc(file);
      if (c == EOF || c == '\n')
        break;
      if (((long)q - (long)b + 1 ) >= (long) len)
        {
          long
            tlen;
          tlen=(long)q-(long)b;
          len<<=1;
          b=(char *) realloc((char *) b,(len+2));
          if ((char *) b == (char *) NULL)
            break;
          q=b+tlen;
        }
      *q=(unsigned char) c;
    }
  *blen=0;
  if ((unsigned char *)b != (unsigned char *) NULL)
    {
      int
        tlen;
      tlen=(long)q - (long)b;
      if (tlen == 0)
        return (char *) NULL;
      b[tlen] = '\0';
      *blen=++tlen;
    }
  return b;
}
