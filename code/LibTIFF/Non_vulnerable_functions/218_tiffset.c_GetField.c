static const TIFFField *
GetField(TIFF *tiff, const char *tagname)
{
    const TIFFField *fip;
    if( atoi(tagname) > 0 )
        fip = TIFFFieldWithTag(tiff, (ttag_t)atoi(tagname));
    else
        fip = TIFFFieldWithName(tiff, tagname);
    if (!fip) {
        fprintf( stderr, "Field name \"%s\" is not recognised.\n", tagname );
        return (TIFFField *)NULL;
    }
    return fip;
}
