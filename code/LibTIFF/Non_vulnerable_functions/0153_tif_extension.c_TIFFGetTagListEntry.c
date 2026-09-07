uint32 TIFFGetTagListEntry( TIFF *tif, int tag_index )
{
    TIFFDirectory* td = &tif->tif_dir;
    if( tag_index < 0 || tag_index >= td->td_customValueCount )
        return (uint32)(-1);
    else
        return td->td_customValues[tag_index].info->field_tag;
}
