void
_TIFFmemset(tdata_t p, int v, tsize_t c)
{
	char* pp = (char*) p;
	while (c > 0) {
		tsize_t chunk = 0x10000 - ((uint32) pp & 0xffff);
		if (chunk > 0xff00)				
			chunk = 0xff00;
		if (chunk > c)					
			chunk = c;
		memset(pp, v, chunk);
		pp = (char*) (chunk + (char huge*) pp);
		c -= chunk;
	}
}
