void
_TIFFmemcpy(void* d, const tdata_t s, tsize_t c)
{
	CopyMemory(d, s, c);
}
