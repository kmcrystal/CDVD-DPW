static void _XTIFFLocalDefaultDirectory(TIFF *tif)
{
	xtiff *xt = XTIFFDIR(tif);
	XTIFFDirectory* xd = &xt->xtif_dir;
	
	_TIFFMergeFieldInfo(tif, xtiffFieldInfo, N(xtiffFieldInfo));
	



	_XTIFFFreeDirectory(xt);	
	_TIFFmemset(xt,0,sizeof(xtiff));
	
	PARENT(xt,vsetfield) =  TIFFMEMBER(tif,vsetfield);
	TIFFMEMBER(tif,vsetfield) = _XTIFFVSetField;
	PARENT(xt,vgetfield) =  TIFFMEMBER(tif,vgetfield);
	TIFFMEMBER(tif,vgetfield) = _XTIFFVGetField;
	


	xd->xd_example_single = 234;
}
