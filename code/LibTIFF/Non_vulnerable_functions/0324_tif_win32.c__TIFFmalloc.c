tdata_t
_TIFFmalloc(tsize_t s)
{
	return ((tdata_t)GlobalAlloc(GMEM_FIXED, s));
}
