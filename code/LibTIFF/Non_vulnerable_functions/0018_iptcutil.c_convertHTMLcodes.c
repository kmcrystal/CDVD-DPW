 */
int convertHTMLcodes(char *s, int len)
{
  if (len <=0 || s==(char*)NULL || *s=='\0')
    return 0;
  if (s[1] == '#')
    {
      int val, o;
      if (sscanf(s,"&#%d;",&val) == 1)
        {
          o = 3;
          while (s[o] != ';')
            {
              o++;
              if (o > 5)
                break;
            }
          if (o < 5)
            strcpy(s+1, s+1+o);
          *s = val;
          return o;
        }
    }
  else
    {
      int
        i,
        codes = sizeof(html_codes) / sizeof(html_code);
      for (i=0; i < codes; i++)
        {
          if (html_codes[i].len <= len)
            if (STRNICMP(s, html_codes[i].code, html_codes[i].len) == 0)
              {
                strcpy(s+1, s+html_codes[i].len);
                *s = html_codes[i].val;
                return html_codes[i].len-1;
              }
        }
    }
  return 0;
}
