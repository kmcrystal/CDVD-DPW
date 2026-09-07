static int
Fax4Decode(TIFF* tif, tidata_t buf, tsize_t occ, tsample_t s)
{
	DECLARE_STATE_2D(tif, sp, "Fax4Decode");
        int line = 0;
	(void) s;
	CACHE_STATE(tif, sp);
	while ((long)occ > 0) {
		a0 = 0;
		RunLength = 0;
		pa = thisrun = sp->curruns;
		pb = sp->refruns;
		b1 = *pb++;
#ifdef FAX3_DEBUG
		printf("\nBitAcc=%08X, BitsAvail = %d\n", BitAcc, BitsAvail);
		printf("-------------------- %d\n", tif->tif_row);
		fflush(stdout);
#endif
		EXPAND2D(EOFG4);
                if (EOLcnt)
                    goto EOFG4;
		(*sp->fill)(buf, thisrun, pa, lastx);
		SETVALUE(0);		
		SWAP(uint32*, sp->curruns, sp->refruns);
		buf += sp->b.rowbytes;
		occ -= sp->b.rowbytes;
                line++;
		continue;
	EOFG4:
                NeedBits16( 13, BADG4 );
        BADG4:
#ifdef FAX3_DEBUG
                if( GetBits(13) != 0x1001 )
                    fputs( "Bad RTC\n", stderr );
#endif                
                ClrBits( 13 );
		(*sp->fill)(buf, thisrun, pa, lastx);
		UNCACHE_STATE(tif, sp);
		return (-1);
	}
	UNCACHE_STATE(tif, sp);
	return (1);
}
