static void
_XTIFFDefaultDirectory(TIFF *tif)
{
	xtiff *xt;
	
	if (!(tif->tif_flags & XTIFF_INITIALIZED))
	{
		xt = _TIFFmalloc(sizeof(xtiff));
		if (!xt)
		{
			
			return;
		}
		_TIFFmemset(xt,0,sizeof(xtiff));
		


		TIFFMEMBER(tif,clientdir) = (tidata_t)xt;
		tif->tif_flags |= XTIFF_INITIALIZED; 
	}
	
	_XTIFFLocalDefaultDirectory(tif);
	



	if (_ParentExtender) 
		(*_ParentExtender)(tif);
}
