double
_TIFFUInt64ToDouble(uint64 ui64)
{
	_Int64 i;
	i.value = ui64;
	if (i.part.high >= 0) {
		return (double)i.value;
	} else {
		long double df;
		df = (long double)i.value;
		df += 18446744073709551616.0; 
		return (double)df;
	}
}
