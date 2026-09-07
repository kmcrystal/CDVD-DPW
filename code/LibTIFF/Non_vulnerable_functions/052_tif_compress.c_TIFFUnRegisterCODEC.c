void
TIFFUnRegisterCODEC(TIFFCodec* c)
{
	codec_t* cd;
	codec_t** pcd;
	for (pcd = &registeredCODECS; (cd = *pcd); pcd = &cd->next)
		if (cd->info == c) {
			*pcd = cd->next;
			_TIFFfree(cd);
			return;
		}
	TIFFErrorExt(0, "TIFFUnRegisterCODEC",
	    "Cannot remove compression scheme %s; not registered", c->name);
}
