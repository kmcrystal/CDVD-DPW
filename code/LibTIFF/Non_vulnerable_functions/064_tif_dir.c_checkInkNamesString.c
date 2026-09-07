static uint32
checkInkNamesString(TIFF* tif, uint32 slen, const char* s)
{
	TIFFDirectory* td = &tif->tif_dir;
	uint16 i = td->td_samplesperpixel;
	if (slen > 0) {
		const char* ep = s+slen;
		const char* cp = s;
		for (; i > 0; i--) {
			for (; *cp != '\0'; cp++)
				if (cp >= ep)
					goto bad;
			cp++;				
		}
		return (cp-s);
	}
bad:
	TIFFErrorExt(tif->tif_clientdata, "TIFFSetField",
	    "%s: Invalid InkNames value; expecting %d names, found %d",
	    tif->tif_name,
	    td->td_samplesperpixel,
	    td->td_samplesperpixel-i);
	return (0);
}
