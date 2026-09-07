static int
CheckShort2Tag(TIFF* tif1, TIFF* tif2, int tag, char* name)
{
	uint16 v11, v12, v21, v22;
	if (TIFFGetField(tif1, tag, &v11, &v12)) {
		if (!TIFFGetField(tif2, tag, &v21, &v22)) {
			printf("%s tag appears only in %s\n",
			    name, TIFFFileName(tif1));
			return (0);
		}
		if (v11 == v21 && v12 == v22)
			return (1);
		printf("%s: <%u,%u> <%u,%u>\n", name, v11, v12, v21, v22);
	} else if (TIFFGetField(tif2, tag, &v21, &v22))
		printf("%s tag appears only in %s\n", name, TIFFFileName(tif2));
	else
		return (1);
	return (0);
}
