 * do with a single TIFFGetOvrBlock and no longer need TIFFGetOvrBlock_Subsampled */
unsigned char *TIFFGetOvrBlock( TIFFOvrCache *psCache, int iTileX, int iTileY,
                                int iSample )
{
    int		nRowOffset;
    if( iTileY > psCache->nBlockOffset + 1 )
        TIFFWriteOvrRow( psCache );
    assert( iTileX >= 0 && iTileX < psCache->nBlocksPerRow );
    assert( iTileY >= 0 && iTileY < psCache->nBlocksPerColumn );
    assert( iTileY >= psCache->nBlockOffset
            && iTileY < psCache->nBlockOffset+2 );
    assert( iSample >= 0 && iSample < psCache->nSamples );
    if (psCache->nPlanarConfig == PLANARCONFIG_SEPARATE)
        nRowOffset = ((iTileX * psCache->nSamples) + iSample)
            * psCache->nBytesPerBlock;
    else
        nRowOffset = iTileX * psCache->nBytesPerBlock +
            (psCache->nBitsPerPixel + 7) / 8 * iSample;
    if( iTileY == psCache->nBlockOffset )
        return psCache->pabyRow1Blocks + nRowOffset;
    else
        return psCache->pabyRow2Blocks + nRowOffset;
}
