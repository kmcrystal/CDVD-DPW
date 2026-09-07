TIFF*
TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	Str255 pname;
	FInfo finfo;
	short fref;
	OSErr err;
	FSSpec	fSpec;
	strcpy((char*) pname, name);
	ourc2pstr((char*) pname);
	err = FSMakeFSSpec( 0, 0, pname, &fSpec );
	switch (_TIFFgetMode(mode, module)) {
	default:
		return ((TIFF*) 0);
	case O_RDWR | O_CREAT | O_TRUNC:
		if (FSpGetFInfo(&fSpec, &finfo) == noErr)
			FSpDelete(&fSpec);
		
	case O_RDWR | O_CREAT:
		if ((err = FSpGetFInfo(&fSpec, &finfo)) == fnfErr) {
			if (FSpCreate(&fSpec, '    ', 'TIFF', smSystemScript) != noErr)
				goto badCreate;
			if (FSpOpenDF(&fSpec, fsRdWrPerm, &fref) != noErr)
				goto badOpen;
		} else if (err == noErr) {
			if (FSpOpenDF(&fSpec, fsRdWrPerm, &fref) != noErr)
				goto badOpen;
		} else
			goto badOpen;
		break;
	case O_RDONLY:
		if (FSpOpenDF(&fSpec, fsRdPerm, &fref) != noErr)
			goto badOpen;
		break;
	case O_RDWR:
		if (FSpOpenDF(&fSpec, fsRdWrPerm, &fref) != noErr)
			goto badOpen;
		break;
	}
	return (TIFFFdOpen((int) fref, name, mode));
badCreate:
	TIFFErrorExt(0, module, "%s: Cannot create", name);
	return ((TIFF*) 0);
badOpen:
	TIFFErrorExt(0, module, "%s: Cannot open", name);
	return ((TIFF*) 0);
}
