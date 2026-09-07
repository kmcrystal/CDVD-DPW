static int
_tiffMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	char name[256];
	struct FAB fab;
	unsigned short channel;
	char *inadr[2], *retadr[2];
	unsigned long status;
	long size;
	if (no_mapped >= MAX_MAPPED)
		return(0);
	



	if (getname((int)fd, name, 1) == NULL)
		return(0);
	
	fab = cc$rms_fab;
	fab.fab$l_fop |= FAB$V_UFO;
	fab.fab$b_fac = FAB$M_GET;
	fab.fab$b_shr = FAB$M_SHRGET;
	fab.fab$l_fna = name;
	fab.fab$b_fns = strlen(name);
	status = sys$open(&fab);	
	if ((status&1) == 0)
		return(0);
	channel = (unsigned short)fab.fab$l_stv;
	inadr[0] = inadr[1] = (char *)0; 
	



	size = _tiffSizeProc(fd);
	status = sys$crmpsc(inadr, retadr, 0, SEC$M_EXPREG, 0,0,0, channel,
		TIFFhowmany(size,512), 0,0,0);  ddd
	if ((status&1) == 0){
		sys$dassgn(channel);
		return(0);
	}
	*pbase = (tdata_t) retadr[0];	
	



	*psize = (toff_t) size;
	
	map_table[no_mapped].base = retadr[0];
	map_table[no_mapped].top = retadr[1];
	map_table[no_mapped].channel = channel;
	no_mapped++;
        return(1);
}
static int
_tiffMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	return (0);
}
