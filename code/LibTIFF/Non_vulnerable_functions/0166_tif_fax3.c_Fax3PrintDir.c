static void
Fax3PrintDir(TIFF* tif, FILE* fd, long flags)
{
	Fax3BaseState* sp = Fax3State(tif);
	(void) flags;
	if (TIFFFieldSet(tif,FIELD_OPTIONS)) {
		const char* sep = " ";
		if (tif->tif_dir.td_compression == COMPRESSION_CCITTFAX4) {
			fprintf(fd, "  Group 4 Options:");
			if (sp->groupoptions & GROUP4OPT_UNCOMPRESSED)
				fprintf(fd, "%suncompressed data", sep);
		} else {
			fprintf(fd, "  Group 3 Options:");
			if (sp->groupoptions & GROUP3OPT_2DENCODING)
				fprintf(fd, "%s2-d encoding", sep), sep = "+";
			if (sp->groupoptions & GROUP3OPT_FILLBITS)
				fprintf(fd, "%sEOL padding", sep), sep = "+";
			if (sp->groupoptions & GROUP3OPT_UNCOMPRESSED)
				fprintf(fd, "%suncompressed data", sep);
		}
		fprintf(fd, " (%lu = 0x%lx)\n",
                        (unsigned long) sp->groupoptions,
                        (unsigned long) sp->groupoptions);
	}
	if (TIFFFieldSet(tif,FIELD_CLEANFAXDATA)) {
		fprintf(fd, "  Fax Data:");
		switch (sp->cleanfaxdata) {
		case CLEANFAXDATA_CLEAN:
			fprintf(fd, " clean");
			break;
		case CLEANFAXDATA_REGENERATED:
			fprintf(fd, " receiver regenerated");
			break;
		case CLEANFAXDATA_UNCLEAN:
			fprintf(fd, " uncorrected errors");
			break;
		}
		fprintf(fd, " (%u = 0x%x)\n",
		    sp->cleanfaxdata, sp->cleanfaxdata);
	}
	if (TIFFFieldSet(tif,FIELD_BADFAXLINES))
		fprintf(fd, "  Bad Fax Lines: %lu\n",
                        (unsigned long) sp->badfaxlines);
	if (TIFFFieldSet(tif,FIELD_BADFAXRUN))
		fprintf(fd, "  Consecutive Bad Fax Lines: %lu\n",
		    (unsigned long) sp->badfaxrun);
	if (TIFFFieldSet(tif,FIELD_RECVPARAMS))
		fprintf(fd, "  Fax Receive Parameters: %08lx\n",
		   (unsigned long) sp->recvparams);
	if (TIFFFieldSet(tif,FIELD_SUBADDRESS))
		fprintf(fd, "  Fax SubAddress: %s\n", sp->subaddress);
	if (TIFFFieldSet(tif,FIELD_RECVTIME))
		fprintf(fd, "  Fax Receive Time: %lu secs\n",
		    (unsigned long) sp->recvtime);
	if (TIFFFieldSet(tif,FIELD_FAXDCS))
		fprintf(fd, "  Fax DCS: %s\n", sp->faxdcs);
}
