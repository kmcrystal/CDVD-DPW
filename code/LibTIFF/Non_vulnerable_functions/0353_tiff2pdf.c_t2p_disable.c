static void
t2p_disable(TIFF *tif)
{
	T2P *t2p = (T2P*) TIFFClientdata(tif);
	t2p->outputdisable = 1;
}
