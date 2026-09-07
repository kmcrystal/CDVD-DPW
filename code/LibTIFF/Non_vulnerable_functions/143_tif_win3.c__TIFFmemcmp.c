int
_TIFFmemcmp(const tdata_t d, const tdata_t s, tsize_t c)
{
	char* dd = (char*) d;
	char* ss = (char*) s;
	tsize_t chunks, chunkd, chunk;
	int result;
	while (c > 0) {
		chunks = 0x10000 - ((uint32) ss & 0xffff);	
		chunkd = 0x10000 - ((uint32) dd & 0xffff);	
		chunk = c;					
		if (chunk > chunks)				
			chunk = chunks;				
		if (chunk > chunkd)
			chunk = chunkd;
		if (chunk > 0xff00)
			chunk = 0xff00;
		result = memcmp(dd, ss, chunk);
		if (result != 0)
			return (result);
		dd = (char*) (chunk + (char huge*) dd);
		ss = (char*) (chunk + (char huge*) ss);
		c -= chunk;
	}
	return (0);
}
