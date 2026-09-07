TIFFErrorHandlerExt
TIFFSetWarningHandlerExt(TIFFErrorHandlerExt handler)
{
	TIFFErrorHandlerExt prev = _TIFFwarningHandlerExt;
	_TIFFwarningHandlerExt = handler;
	return (prev);
}
