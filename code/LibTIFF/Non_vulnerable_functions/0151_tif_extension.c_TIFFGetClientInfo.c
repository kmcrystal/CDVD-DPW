void *TIFFGetClientInfo( TIFF *tif, const char *name )
{
    TIFFClientInfoLink *psLink = tif->tif_clientinfo;
    while( psLink != NULL && strcmp(psLink->name,name) != 0 )
        psLink = psLink->next;
    if( psLink != NULL )
        return psLink->data;
    else
        return NULL;
}
