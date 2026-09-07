TIFFExtendProc
TIFFSetTagExtender(TIFFExtendProc extender)
{
	TIFFExtendProc prev = _TIFFextender;
	_TIFFextender = extender;
	return (prev);
}
