void
_TIFFmemset(void* p, int v, tsize_t c)
{
	FillMemory(p, c, (BYTE)v);
}
