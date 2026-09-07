void*
_TIFFmalloc(tsize_t s)
{
	return (malloc((size_t) s));
}
