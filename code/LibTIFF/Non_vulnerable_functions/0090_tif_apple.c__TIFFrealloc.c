tdata_t
_TIFFrealloc(tdata_t p, tsize_t s)
{
	Ptr n = p;
	SetPtrSize(p, (size_t) s);
	if (MemError() && (n = NewPtr((size_t) s)) != NULL) {
		BlockMove(p, n, GetPtrSize(p));
		DisposePtr(p);
	}
	return ((tdata_t) n);
}
