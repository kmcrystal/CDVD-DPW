void
_TIFFmemcpy(tdata_t d, const tdata_t s, tsize_t c)
{
	if (c > 0xFFFF)
		hmemcpy((void _huge*) d, (void _huge*) s, c);
	else
		(void) memcpy(d, s, (size_t) c);
}
