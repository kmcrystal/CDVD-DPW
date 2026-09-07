tdata_t
_TIFFrealloc(tdata_t p, tsize_t s)
{
	return (tdata_t) GlobalReAllocPtr(p, (DWORD) s, GHND);
}
