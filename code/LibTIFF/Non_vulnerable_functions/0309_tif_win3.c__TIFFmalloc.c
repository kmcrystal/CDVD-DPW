tdata_t
_TIFFmalloc(tsize_t s)
{
	return (tdata_t) GlobalAllocPtr(GHND, (DWORD) s);
}
