int
TIFFReassignTagToIgnore (enum TIFFIgnoreSense task, int TIFFtagID)
{
    static int TIFFignoretags [FIELD_LAST];
    static int tagcount = 0 ;
    int		i;					
    int		j;					
    switch (task)
    {
      case TIS_STORE:
        if ( tagcount < (FIELD_LAST - 1) )
        {
            for ( j = 0 ; j < tagcount ; ++j )
            {					
                if ( TIFFignoretags [j] == TIFFtagID )
                    return (TRUE) ;
            }
            TIFFignoretags [tagcount++] = TIFFtagID ;
            return (TRUE) ;
        }
        break ;
      case TIS_EXTRACT:
        for ( i = 0 ; i < tagcount ; ++i )
        {
            if ( TIFFignoretags [i] == TIFFtagID )
                return (TRUE) ;
        }
        break;
      case TIS_EMPTY:
        tagcount = 0 ;			
        return (TRUE) ;
      default:
        break;
    }
    return (FALSE);
}
