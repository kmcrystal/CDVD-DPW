
int sindex(char ch,char *string)
{
  char *cp;
  for(cp=string;*cp;++cp)
    if(ch==*cp)
      return (int)(cp-string);	
  return -1;			
}
