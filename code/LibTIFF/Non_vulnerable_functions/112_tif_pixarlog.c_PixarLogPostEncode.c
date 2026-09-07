static int
PixarLogPostEncode(TIFF* tif)
{
	static const char module[] = "PixarLogPostEncode";
	PixarLogState *sp = EncoderState(tif);
	int state;
	sp->stream.avail_in = 0;
	do {
		state = deflate(&sp->stream, Z_FINISH);
		switch (state) {
		case Z_STREAM_END:
		case Z_OK:
		    if ((tmsize_t)sp->stream.avail_out != tif->tif_rawdatasize) {
			    tif->tif_rawcc =
				tif->tif_rawdatasize - sp->stream.avail_out;
			    TIFFFlushData1(tif);
			    sp->stream.next_out = tif->tif_rawdata;
			    sp->stream.avail_out = (uInt) tif->tif_rawdatasize;  
		    }
		    break;
		default:
			TIFFErrorExt(tif->tif_clientdata, module, "ZLib error: %s",
			sp->stream.msg ? sp->stream.msg : "(null)");
		    return (0);
		}
	} while (state != Z_STREAM_END);
	return (1);
}
