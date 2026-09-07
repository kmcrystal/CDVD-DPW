static
void _XTIFFInitialize(void)
{
	static first_time=1;
	if (! first_time) return; 
	first_time = 0;
	
	_ParentExtender = TIFFSetTagExtender(_XTIFFDefaultDirectory);
}
