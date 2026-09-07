void
TIFFCleanup(TIFF* tif)
{
	if (tif->tif_mode != O_RDONLY)
	    


	    TIFFFlush(tif);
	(*tif->tif_cleanup)(tif);
	TIFFFreeDirectory(tif);
	if (tif->tif_dirlist)
		_TIFFfree(tif->tif_dirlist);
	
	while( tif->tif_clientinfo )
	{
		TIFFClientInfoLink *link = tif->tif_clientinfo;
		tif->tif_clientinfo = link->next;
		_TIFFfree( link->name );
		_TIFFfree( link );
	}
	if (tif->tif_rawdata && (tif->tif_flags&TIFF_MYBUFFER))
		_TIFFfree(tif->tif_rawdata);
	if (isMapped(tif))
		TIFFUnmapFileContents(tif, tif->tif_base, tif->tif_size);
	
	if (tif->tif_nfields > 0)
	{
		size_t  i;
	    for (i = 0; i < tif->tif_nfields; i++) 
	    {
		TIFFFieldInfo *fld = tif->tif_fieldinfo[i];
		if (fld->field_bit == FIELD_CUSTOM && 
		    strncmp("Tag ", fld->field_name, 4) == 0) 
		{
		    _TIFFfree(fld->field_name);
		    _TIFFfree(fld);
		}
	    }   
	    _TIFFfree(tif->tif_fieldinfo);
	}
	_TIFFfree(tif);
}
