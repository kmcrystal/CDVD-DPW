const TIFFCodec*
TIFFFindCODEC(uint16 scheme)
{
	const TIFFCodec* c;
	codec_t* cd;
	for (cd = registeredCODECS; cd; cd = cd->next)
		if (cd->info->scheme == scheme)
			return ((const TIFFCodec*) cd->info);
	for (c = _TIFFBuiltinCODECS; c->name; c++)
		if (c->scheme == scheme)
			return (c);
	return ((const TIFFCodec*) 0);
}
