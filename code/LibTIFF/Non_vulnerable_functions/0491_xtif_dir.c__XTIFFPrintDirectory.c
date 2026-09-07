static void
_XTIFFPrintDirectory(TIFF* tif, FILE* fd, long flags)
{
	xtiff *xt = XTIFFDIR(tif);
	XTIFFDirectory *xd = &xt->xtif_dir;
	int i,num;
	
	if (PARENT(xt,printdir))
		(PARENT(xt,printdir))(tif,fd,flags);
	


	fprintf(fd,"--My Example Tags--\n");
	



	if (TIFFFieldSet(tif,FIELD_EXAMPLE_MULTI))
	{
		fprintf(fd, "  My Multi-Valued Doubles:");
		if (flags & TIFFPRINT_MYMULTIDOUBLES) 
		{
			double *value = xd->xd_example_multi;
			num = xd->xd_num_multi;
			fprintf(fd,"(");
			for (i=0;i<num;i++) fprintf(fd, " %lg", *value++);
			fprintf(fd,")\n");
		} else
			fprintf(fd, "(present)\n");
	}
	if (TIFFFieldSet(tif,FIELD_EXAMPLE_SINGLE))
	{
		fprintf(fd, "  My Single Long Tag:  %lu\n", xd->xd_example_single);
	}
	if (TIFFFieldSet(tif,FIELD_EXAMPLE_ASCII))
	{
		_TIFFprintAsciiTag(fd,"My ASCII Tag",
			 xd->xd_example_ascii);
	}
}
