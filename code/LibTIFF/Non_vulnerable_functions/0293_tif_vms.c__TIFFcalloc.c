void* _TIFFcalloc(tmsize_t nmemb, tmsize_t siz)
{
    if( nmemb == 0 || siz == 0 )
        return ((void *) NULL);
    return calloc((size_t) nmemb, (size_t)siz);
}
