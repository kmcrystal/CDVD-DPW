 */
void formatString(FILE *ofile, const char *s, int len)
{
  putc('"', ofile);
  for (; len > 0; --len, ++s) {
    int c = *s;
    switch (c) {
    case '&':
      fputs("&amp;", ofile);
      break;
#ifdef HANDLE_GT_LT
    case '<':
      fputs("&lt;", ofile);
      break;
    case '>':
      fputs("&gt;", ofile);
      break;
#endif
    case '"':
      fputs("&quot;", ofile);
      break;
    default:
      if (iscntrl(c))
        fprintf(ofile, "&#%d;", c);
      else
        putc(*s, ofile);
      break;
    }
  }
  fputs("\"\n", ofile);
}
