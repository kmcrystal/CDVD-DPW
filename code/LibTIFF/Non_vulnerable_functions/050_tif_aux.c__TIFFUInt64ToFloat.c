float
_TIFFUInt64ToFloat(uint64 ui64)
{
	_Int64 i;
	i.value = ui64;
	if (i.part.high >= 0) {
		return (float)i.value;
	} else {
		long double df;
		df = (long double)i.value;
		df += 18446744073709551616.0; 
		return (float)df;
	}
}
