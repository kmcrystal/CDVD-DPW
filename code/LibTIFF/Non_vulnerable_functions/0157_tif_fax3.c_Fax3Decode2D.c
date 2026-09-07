static int
Fax3Decode2D(TIFF* tif, tidata_t buf, tsize_t occ, tsample_t s)
{
	DECLARE_STATE_2D(tif, sp, "Fax3Decode2D");
        int line = 0;
	int is1D;			
	(void) s;
	CACHE_STATE(tif, sp);
	while ((long)occ > 0) {
		a0 = 0;
		RunLength = 0;
		pa = thisrun = sp->curruns;
#ifdef FAX3_DEBUG
		printf("\nBitAcc=%08X, BitsAvail = %d EOLcnt = %d",
		    BitAcc, BitsAvail, EOLcnt);
#endif
		SYNC_EOL(EOF2D);
		NeedBits8(1, EOF2D);
		is1D = GetBits(1);	
		ClrBits(1);
#ifdef FAX3_DEBUG
		printf(" %s\n-------------------- %d\n",
		    is1D ? "1D" : "2D", tif->tif_row);
		fflush(stdout);
#endif
		pb = sp->refruns;
		b1 = *pb++;
		if (is1D)
			EXPAND1D(EOF2Da);
		else
			EXPAND2D(EOF2Da);
		(*sp->fill)(buf, thisrun, pa, lastx);
		SETVALUE(0);		
		SWAP(uint32*, sp->curruns, sp->refruns);
		buf += sp->b.rowbytes;
		occ -= sp->b.rowbytes;
                line++;
		continue;
	EOF2D:				
		CLEANUP_RUNS();
	EOF2Da:				
		(*sp->fill)(buf, thisrun, pa, lastx);
		UNCACHE_STATE(tif, sp);
		return (-1);
	}
	UNCACHE_STATE(tif, sp);
	return (1);
}
