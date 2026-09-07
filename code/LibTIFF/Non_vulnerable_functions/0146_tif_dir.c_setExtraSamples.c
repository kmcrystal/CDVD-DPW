static int
setExtraSamples(TIFFDirectory* td, va_list ap, uint32* v)
{
	uint16* va;
	uint32 i;
	*v = va_arg(ap, uint32);
	if ((uint16) *v > td->td_samplesperpixel)
		return (0);
	va = va_arg(ap, uint16*);
	if (*v > 0 && va == NULL)		
		return (0);
	for (i = 0; i < *v; i++)
		if (va[i] > EXTRASAMPLE_UNASSALPHA)
			return (0);
	td->td_extrasamples = (uint16) *v;
	_TIFFsetShortArray(&td->td_sampleinfo, va, td->td_extrasamples);
	return (1);
}
