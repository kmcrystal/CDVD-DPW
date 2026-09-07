void
WriteTable(FILE* fd, const TIFFFaxTabEnt* T, int Size, const char* name)
{
    int i;
    char* sep;
    fprintf(fd, "%s %s TIFFFaxTabEnt %s[%d] = {",
	storage_class, const_class, name, Size);
    if (packoutput) {
	sep = "\n";
	for (i = 0; i < Size; i++) {
	    fprintf(fd, "%s%s%d,%d,%d%s",
		sep, prebrace, T->State, T->Width, (int) T->Param, postbrace);
	    if (((i+1) % 10) == 0)
		    sep = ",\n";
	    else
		    sep = ",";
	    T++;
	}
    } else {
	sep = "\n ";
	for (i = 0; i < Size; i++) {
	    fprintf(fd, "%s%s%3d,%3d,%4d%s",
		sep, prebrace, T->State, T->Width, (int) T->Param, postbrace);
	    if (((i+1) % 6) == 0)
		    sep = ",\n ";
	    else
		    sep = ",";
	    T++;
	}
    }
    fprintf(fd, "\n};\n");
}
