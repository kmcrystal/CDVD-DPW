static void
t2p_enable(TIFF *tif)
{
	T2P *t2p = (T2P*) TIFFClientdata(tif);
	t2p->outputdisable = 0;
}
