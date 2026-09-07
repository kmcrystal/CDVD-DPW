static void
PixarLogClose(TIFF* tif)
{
        PixarLogState* sp = (PixarLogState*) tif->tif_data;
	TIFFDirectory *td = &tif->tif_dir;
	assert(sp != 0);
	







        if (sp->state&PLSTATE_INIT) {
            







            td->td_bitspersample = 8;
            td->td_sampleformat = SAMPLEFORMAT_UINT;
        }
}
