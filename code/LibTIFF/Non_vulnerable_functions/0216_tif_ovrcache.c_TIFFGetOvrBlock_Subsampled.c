
unsigned char *TIFFGetOvrBlock_Subsampled( TIFFOvrCache *psCache, 
                                           int iTileX, int iTileY )
{
    int		nRowOffset;
    if( iTileY > psCache->nBlockOffset + 1 )
        TIFFWriteOvrRow( psCache );
    assert( iTileX >= 0 && iTileX < psCache->nBlocksPerRow );
    assert( iTileY >= 0 && iTileY < psCache->nBlocksPerColumn );
    assert( iTileY >= psCache->nBlockOffset
            && iTileY < psCache->nBlockOffset+2 );
    assert( psCache->nPlanarConfig != PLANARCONFIG_SEPARATE );
    nRowOffset = iTileX * psCache->nBytesPerBlock;
    if( iTileY == psCache->nBlockOffset )
        return psCache->pabyRow1Blocks + nRowOffset;
    else
        return psCache->pabyRow2Blocks + nRowOffset;
}
