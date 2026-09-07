TIFFCodec*
TIFFGetConfiguredCODECs()
{
	int		i = 1;
        codec_t		*cd;
        const TIFFCodec	*c;
	TIFFCodec	*codecs = NULL, *new_codecs;
        for (cd = registeredCODECS; cd; cd = cd->next) {
                new_codecs = (TIFFCodec *)
			_TIFFrealloc(codecs, i * sizeof(TIFFCodec));
		if (!new_codecs) {
			_TIFFfree (codecs);
			return NULL;
		}
		codecs = new_codecs;
		_TIFFmemcpy(codecs + i - 1, cd, sizeof(TIFFCodec));
		i++;
	}
        for (c = _TIFFBuiltinCODECS; c->name; c++) {
                if (TIFFIsCODECConfigured(c->scheme)) {
                        new_codecs = (TIFFCodec *)
				_TIFFrealloc(codecs, i * sizeof(TIFFCodec));
			if (!new_codecs) {
				_TIFFfree (codecs);
				return NULL;
			}
			codecs = new_codecs;
			_TIFFmemcpy(codecs + i - 1, (const tdata_t)c, sizeof(TIFFCodec));
			i++;
		}
	}
	new_codecs = (TIFFCodec *) _TIFFrealloc(codecs, i * sizeof(TIFFCodec));
	if (!new_codecs) {
		_TIFFfree (codecs);
		return NULL;
	}
	codecs = new_codecs;
	_TIFFmemset(codecs + i - 1, 0, sizeof(TIFFCodec));
        return codecs;
}
