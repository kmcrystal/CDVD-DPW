
void TIFFDestroyOvrCache( TIFFOvrCache * psCache )
{
    while( psCache->nBlockOffset < psCache->nBlocksPerColumn )
        TIFFWriteOvrRow( psCache );
    _TIFFfree( psCache->pabyRow1Blocks );
    _TIFFfree( psCache->pabyRow2Blocks );
    _TIFFfree( psCache );
}
