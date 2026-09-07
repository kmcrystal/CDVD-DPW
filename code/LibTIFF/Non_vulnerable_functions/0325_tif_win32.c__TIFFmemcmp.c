int
_TIFFmemcmp(const tdata_t p1, const tdata_t p2, tsize_t c)
{
	register const BYTE *pb1 = (const BYTE *) p1;
	register const BYTE *pb2 = (const BYTE *) p2;
	register DWORD dwTmp = c;
	register int iTmp;
	for (iTmp = 0; dwTmp-- && !iTmp; iTmp = (int)*pb1++ - (int)*pb2++)
		;
	return (iTmp);
}
