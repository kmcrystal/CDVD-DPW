int formatIPTC(FILE *ifile, FILE *ofile)
{
  unsigned int
    foundiptc,
    tagsfound;
  char
    *readable,
    *str;
  long
    tagindx,
    taglen;
  int
    i,
    tagcount = sizeof(tags) / sizeof(tag_spec);
  int
    c,
    dataset,
    recnum;
  foundiptc = 0; 
  tagsfound = 0; 
  c = getc(ifile);
  while (c != EOF)
    {
      if (c == 0x1c)
        foundiptc = 1;
      else
        {
          if (foundiptc)
            {
              return -1;
            }
          else
            {
              c = getc(ifile);
              continue;
            }
        }
      
      dataset = getc(ifile);
      if ((char) dataset == EOF)
        return -1;
      recnum = getc(ifile);
      if ((char) recnum == EOF)
        return -1;
      
      for (i=0; i< tagcount; i++)
        {
          if (tags[i].id == recnum)
            break;
        }
      if (i < tagcount)
        readable = tags[i].name;
      else
        readable = "";
      
      c = getc(ifile);
      if (c == EOF)
        return 0;
      if (c & (unsigned char) 0x80)
        {
          unsigned char
            buffer[4];
          for (i=0; i<4; i++)
            {
              c = getc(ifile);
              if (c == EOF)
                return -1;
              buffer[i] = c;
            }
          taglen = (((long) buffer[ 0 ]) << 24) |
            (((long) buffer[ 1 ]) << 16) | 
            (((long) buffer[ 2 ]) <<  8) |
            (((long) buffer[ 3 ]));
        }
      else
        {
          int
            x = c;
          taglen = x << 8;
          x = getc(ifile);
          if (x == EOF)
            return -1;
          taglen |= (long) x;
        }
      
      if ((taglen <= 0) || (taglen > 1048576))
        {
          printf("Inappropriate IPTC tag length %ld\n",taglen);
          return -1;
        }
      
      str = (char *) malloc((unsigned int) (taglen+1));
      if (str == (char *) NULL)
        {
          printf("Memory allocation failed");
          return 0;
        }
      for (tagindx=0; tagindx<taglen; tagindx++)
        {
          c = getc(ifile);
          if (c == EOF)
            {
              free(str);
              return -1;
            }
          str[tagindx] = c;
        }
      str[ taglen ] = 0;
      
      if (strlen(readable) > 0)
        fprintf(ofile, "%d#%d#%s=",(unsigned int)dataset, (unsigned int) recnum, readable);
      else
        fprintf(ofile, "%d#%d=",(unsigned int)dataset, (unsigned int) recnum);
      formatString( ofile, str, taglen );
      free(str);
      tagsfound++;
      c = getc(ifile);
    }
  return tagsfound;
}
