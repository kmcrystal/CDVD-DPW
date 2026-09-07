int main(int argc, char *argv[])
{            
  
  
  

  int
    i,
    mode; 
  FILE
    *ifile = stdin,
    *ofile = stdout;
  char
    c,
    *usage = "usage: iptcutil -t | -b [-i file] [-o file] <input >output";
  if( argc < 2 )
    {
      puts(usage);
      return 1;
    }
  mode = 0;
  
  
  for (i=1; i<argc; i++)
    {
      c = argv[i][0];
      if (c == '-' || c == '/')
        {
          c = argv[i][1];
          switch( c )
            {
            case 't':
              mode = 1;
#ifdef WIN32
              
              _setmode( _fileno( ofile ), _O_BINARY );
#endif
              break;
            case 'b':
              mode = 0;
#ifdef WIN32
              
              _setmode( _fileno( ifile ), _O_BINARY );
#endif
              break;
            case 'i':
              if (mode == 0)
                ifile = fopen(argv[++i], "rb");
              else
                ifile = fopen(argv[++i], "rt");
              if (ifile == (FILE *)NULL)
                {
                  printf("Unable to open: %s\n", argv[i]);
                  return 1;
                }
              break;
            case 'o':
              if (mode == 0)
                ofile = fopen(argv[++i], "wt");
              else
                ofile = fopen(argv[++i], "wb");
              if (ofile == (FILE *)NULL)
                {
                  printf("Unable to open: %s\n", argv[i]);
                  return 1;
                }
              break;
            default:
              printf("Unknown option: %s\n", argv[i]);
              return 1;
            }
        }
      else
        {
          puts(usage);
          return 1;
        }
    }
  if (mode == 0) 
    formatIPTC(ifile, ofile);
  if (mode == 1) 
    {
      char
        brkused,
        quoted,
        *line,
        *token,
        *newstr;
      int
        state,
        next;
      unsigned char
        recnum = 0,
        dataset = 0;
      int
        inputlen = BUFFER_SZ;
      line = (char *) malloc(inputlen);     
      token = (char *)NULL;
      while((line = super_fgets(line,&inputlen,ifile))!=NULL)
        {
          state=0;
          next=0;
          token = (char *) malloc(inputlen);     
          newstr = (char *) malloc(inputlen);     
          while(tokenizer(0, token, inputlen, line, "", "=", "\"", 0,
                          &brkused,&next,&quoted)==0)
            {
              if (state == 0)
                {                  
                  int
                    state,
                    next;
                  char
                    brkused,
                    quoted;
                  state=0;
                  next=0;
                  while(tokenizer(0, newstr, inputlen, token, "", "#", "", 0,
                                  &brkused, &next, &quoted)==0)
                    {
                      if (state == 0)
                        dataset = (unsigned char) atoi(newstr);
                      else
                        if (state == 1)
                          recnum = (unsigned char) atoi(newstr);
                      state++;
                    }
                }
              else
                if (state == 1)
                  {
                    int
                      next;
                    unsigned long
                      len;
                    char
                      brkused,
                      quoted;
                    next=0;
                    len = strlen(token);
                    while(tokenizer(0, newstr, inputlen, token, "", "&", "", 0,
                                    &brkused, &next, &quoted)==0)
                      {
                        if (brkused && next > 0)
                          {
                            char
                              *s = &token[next-1];
                            len -= convertHTMLcodes(s, strlen(s));
                          }
                      }
                    fputc(0x1c, ofile);
                    fputc(dataset, ofile);
                    fputc(recnum, ofile);
                    if (len < 0x10000)
                      {
                        fputc((len >> 8) & 255, ofile);
                        fputc(len & 255, ofile);
                      }
                    else
                      {
                        fputc(((len >> 24) & 255) | 0x80, ofile);
                        fputc((len >> 16) & 255, ofile);
                        fputc((len >> 8) & 255, ofile);
                        fputc(len & 255, ofile);
                      }
                    next=0;
                    while (len--)
                      fputc(token[next++], ofile);
                  }
              state++;
            }
          free(token);
          token = (char *)NULL;
          free(newstr);
          newstr = (char *)NULL;
        }
      free(line);
      fclose( ifile );
      fclose( ofile );
    }
  return 0;
}
