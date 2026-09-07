
int main( int argc, char ** argv )
{
    int		anOverviews[100];   
    int		nOverviewCount = 0;
    int		bUseSubIFD = 0;
    TIFF	*hTIFF;
    const char  *pszResampling = "nearest";



    if( argc < 2 )
    {
        printf( "Usage: addtiffo [-r {nearest,average,mode}]\n"
                "                tiff_filename [resolution_reductions]\n"
                "\n"
                "Example:\n"
                " %% addtiffo abc.tif 2 4 8 16\n" );
        return( 1 );
    }
    while( argv[1][0] == '-' )
    {
        if( strcmp(argv[1],"-subifd") == 0 )
        {
            bUseSubIFD = 1;
            argv++;
            argc--;
        }
        else if( strcmp(argv[1],"-r") == 0 )
        {
            argv += 2;
            argc -= 2;
            pszResampling = *argv;
        }
        else
        {
            fprintf( stderr, "Incorrect parameters\n" );
            return( 1 );
        }
    }
    



    while( nOverviewCount < argc - 2 && nOverviewCount < 100 )
    {
        anOverviews[nOverviewCount] = atoi(argv[nOverviewCount+2]);
        if( anOverviews[nOverviewCount] <= 0)
        {
            fprintf( stderr, "Incorrect parameters\n" );
            return(1);
        }
        nOverviewCount++;
    }




    
    if( nOverviewCount == 0 )
    {
        nOverviewCount = 4;
        anOverviews[0] = 2;
        anOverviews[1] = 4;
        anOverviews[2] = 8;
        anOverviews[3] = 16;
    }



    hTIFF = TIFFOpen( argv[1], "r+" );
    if( hTIFF == NULL )
    {
        fprintf( stderr, "TIFFOpen(%s) failed.\n", argv[1] );
        return( 1 );
    }
    TIFFBuildOverviews( hTIFF, nOverviewCount, anOverviews, bUseSubIFD,
                        pszResampling, NULL, NULL );
    TIFFClose( hTIFF );



#ifdef DBMALLOC
    malloc_dump(1);
#endif
    return( 0 );
}
