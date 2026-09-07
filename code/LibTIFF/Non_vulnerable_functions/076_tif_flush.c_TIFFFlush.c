int
TIFFFlush(TIFF* tif)
{
    if( tif->tif_mode == O_RDONLY )
        return 1;
    if (!TIFFFlushData(tif))
        return (0);
    



    if( (tif->tif_flags & TIFF_DIRTYSTRIP)
        && !(tif->tif_flags & TIFF_DIRTYDIRECT) 
        && tif->tif_mode == O_RDWR )
    {
        uint64  *offsets=NULL, *sizes=NULL;
        if( TIFFIsTiled(tif) )
        {
            if( TIFFGetField( tif, TIFFTAG_TILEOFFSETS, &offsets ) 
                && TIFFGetField( tif, TIFFTAG_TILEBYTECOUNTS, &sizes ) 
                && _TIFFRewriteField( tif, TIFFTAG_TILEOFFSETS, TIFF_LONG8, 
                                      tif->tif_dir.td_nstrips, offsets )
                && _TIFFRewriteField( tif, TIFFTAG_TILEBYTECOUNTS, TIFF_LONG8, 
                                      tif->tif_dir.td_nstrips, sizes ) )
            {
                tif->tif_flags &= ~TIFF_DIRTYSTRIP;
                tif->tif_flags &= ~TIFF_BEENWRITING;
                return 1;
            }
        }
        else
        {
            if( TIFFGetField( tif, TIFFTAG_STRIPOFFSETS, &offsets ) 
                && TIFFGetField( tif, TIFFTAG_STRIPBYTECOUNTS, &sizes ) 
                && _TIFFRewriteField( tif, TIFFTAG_STRIPOFFSETS, TIFF_LONG8, 
                                      tif->tif_dir.td_nstrips, offsets )
                && _TIFFRewriteField( tif, TIFFTAG_STRIPBYTECOUNTS, TIFF_LONG8, 
                                      tif->tif_dir.td_nstrips, sizes ) )
            {
                tif->tif_flags &= ~TIFF_DIRTYSTRIP;
                tif->tif_flags &= ~TIFF_BEENWRITING;
                return 1;
            }
        }
    }
    if ((tif->tif_flags & (TIFF_DIRTYDIRECT|TIFF_DIRTYSTRIP)) 
        && !TIFFRewriteDirectory(tif))
        return (0);
    return (1);
}
