TIFFErrorHandler
TIFFSetWarningHandler(TIFFErrorHandler handler)
{
	TIFFErrorHandler prev = _TIFFwarningHandler;
	_TIFFwarningHandler = handler;
	return (prev);
}
