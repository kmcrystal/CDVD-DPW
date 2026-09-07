
void chstore(char *string,int max,char ch)
{
  char c;
  if(_p_tokpos>=0&&_p_tokpos<max-1)
    {
      if(_p_state==IN_QUOTE)
        c=ch;
      else
        switch(_p_flag&3)
          {
          case 1: 	    
            c=toupper((int) ch);
            break;
          case 2: 	    
            c=tolower((int) ch);
            break;
          default:	    
            c=ch;
            break;
          }
      string[_p_tokpos++]=c;
    }
  return;
}
