tdata_t
_TIFFrealloc(tdata_t p, tsize_t s)
{
        void* pvTmp;
        tsize_t old;
        if(p == NULL)
                return ((tdata_t)GlobalAlloc(GMEM_FIXED, s));
        old = GlobalSize(p);
        if (old>=s) {
                if ((pvTmp = GlobalAlloc(GMEM_FIXED, s)) != NULL) {
	                CopyMemory(pvTmp, p, s);
	                GlobalFree(p);
                }
        } else {
                if ((pvTmp = GlobalAlloc(GMEM_FIXED, s)) != NULL) {
	                CopyMemory(pvTmp, p, old);
	                GlobalFree(p);
                }
        }
        return ((tdata_t)pvTmp);
}
