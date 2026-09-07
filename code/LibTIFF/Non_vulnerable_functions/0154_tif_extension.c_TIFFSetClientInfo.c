void TIFFSetClientInfo( TIFF *tif, void *data, const char *name )
{
    TIFFClientInfoLink *psLink = tif->tif_clientinfo;
    



    while( psLink != NULL && strcmp(psLink->name,name) != 0 )
        psLink = psLink->next;
    if( psLink != NULL )
    {
        psLink->data = data;
        return;
    }
    


    psLink = (TIFFClientInfoLink *) _TIFFmalloc(sizeof(TIFFClientInfoLink));
    assert (psLink != NULL);
    psLink->next = tif->tif_clientinfo;
    psLink->name = (char *) _TIFFmalloc((tmsize_t)(strlen(name)+1));
    assert (psLink->name != NULL);
    strcpy(psLink->name, name);
    psLink->data = data;
    tif->tif_clientinfo = psLink;
}
