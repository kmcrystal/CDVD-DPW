int tokenizer(unsigned inflag,char *token,int tokmax,char *line,
              char *white,char *brkchar,char *quote,char eschar,char *brkused,
              int *next,char *quoted)
{
  int qp;
  char c,nc;
  *brkused=0;		
  *quoted=0;		
  if(!line[*next])	
    return 1;
  _p_state=IN_WHITE;   
  _p_curquote=0;	   
  _p_flag=inflag;	   
  for(_p_tokpos=0;(c=line[*next]);++(*next))	
    {
      if((qp=sindex(c,brkchar))>=0)  
        {
          switch(_p_state)
            {
	    case IN_WHITE:		
	    case IN_TOKEN:		
	    case IN_OZONE:		
	      ++(*next);
	      *brkused=brkchar[qp];
	      goto byebye;
	    case IN_QUOTE:		 
	      chstore(token,tokmax,c);
	      break;
            }
        }
      else if((qp=sindex(c,quote))>=0)  
        {
          switch(_p_state)
            {
	    case IN_WHITE:	 
	      _p_state=IN_QUOTE; 
	      _p_curquote=quote[qp]; 
	      *quoted=1;	
	      break;
	    case IN_QUOTE:
	      if(quote[qp]==_p_curquote) 
                {
                  _p_state=IN_OZONE;
                  _p_curquote=0;
                }
	      else
	        chstore(token,tokmax,c); 
	      break;
	    case IN_TOKEN:
	    case IN_OZONE:
	      *brkused=c; 
	      goto byebye;
            }
        }
      else if((qp=sindex(c,white))>=0) 
        {
          switch(_p_state)
            {
	    case IN_WHITE:
	    case IN_OZONE:
	      break;		
	    case IN_TOKEN:
	      _p_state=IN_OZONE;
	      break;
	    case IN_QUOTE:
	      chstore(token,tokmax,c); 
	      break;
            }
        }
      else if(c==eschar)  
        {
          nc=line[(*next)+1];
          if(nc==0) 		
            {
              *brkused=0;
              chstore(token,tokmax,c);
              ++(*next);
              goto byebye;
            }
          switch(_p_state)
            {
	    case IN_WHITE:
	      --(*next);
	      _p_state=IN_TOKEN;
	      break;
	    case IN_TOKEN:
	    case IN_QUOTE:
	      ++(*next);
	      chstore(token,tokmax,nc);
	      break;
	    case IN_OZONE:
	      goto byebye;
            }
        }
      else	
        {
          switch(_p_state)
            {
	    case IN_WHITE:
	      _p_state=IN_TOKEN; 
	    case IN_TOKEN:		 
	    case IN_QUOTE:		 
	      chstore(token,tokmax,c);
	      break;
	    case IN_OZONE:
	      goto byebye;
            }
        }
    }		
 byebye:
  token[_p_tokpos]=0;	
  return 0;
}
