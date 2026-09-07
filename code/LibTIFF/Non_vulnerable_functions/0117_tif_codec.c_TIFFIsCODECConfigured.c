int
TIFFIsCODECConfigured(uint16 scheme)
{
	const TIFFCodec* codec = TIFFFindCODEC(scheme);
	if(codec == NULL) {
		return 0;
	}
	if(codec->init == NULL) {
		return 0;
	}
	if(codec->init != NotConfigured){
		return 1;
	}
	return 0;
}
