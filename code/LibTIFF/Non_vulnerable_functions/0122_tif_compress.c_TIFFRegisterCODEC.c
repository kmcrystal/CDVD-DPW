TIFFCodec*
TIFFRegisterCODEC(uint16 scheme, const char* name, TIFFInitMethod init)
{
	codec_t* cd = (codec_t*)
	    _TIFFmalloc(sizeof (codec_t) + sizeof (TIFFCodec) + strlen(name)+1);
	if (cd != NULL) {
		cd->info = (TIFFCodec*) ((tidata_t) cd + sizeof (codec_t));
		cd->info->name = (char*)
		    ((tidata_t) cd->info + sizeof (TIFFCodec));
		strcpy(cd->info->name, name);
		cd->info->scheme = scheme;
		cd->info->init = init;
		cd->next = registeredCODECS;
		registeredCODECS = cd;
	} else {
		TIFFErrorExt(0, "TIFFRegisterCODEC",
		    "No space to register compression scheme %s", name);
		return NULL;
	}
	return (cd->info);
}
