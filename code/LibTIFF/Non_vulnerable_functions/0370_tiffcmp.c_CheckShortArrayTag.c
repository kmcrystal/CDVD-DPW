static int
CheckShortArrayTag(TIFF* tif1, TIFF* tif2, int tag, char* name)
{
	uint16 n1, *a1;
	uint16 n2, *a2;
	if (TIFFGetField(tif1, tag, &n1, &a1)) {
		if (!TIFFGetField(tif2, tag, &n2, &a2)) {
			printf("%s tag appears only in %s\n",
			    name, TIFFFileName(tif1));
			return (0);
		}
		if (n1 == n2) {
			char* sep;
			uint16 i;
			if (memcmp(a1, a2, n1 * sizeof(uint16)) == 0)
				return (1);
			printf("%s: value mismatch, <%u:", name, n1);
			sep = "";
			for (i = 0; i < n1; i++)
				printf("%s%u", sep, a1[i]), sep = ",";
			printf("> and <%u: ", n2);
			sep = "";
			for (i = 0; i < n2; i++)
				printf("%s%u", sep, a2[i]), sep = ",";
			printf(">\n");
		} else
			printf("%s: %u items in %s, %u items in %s", name,
			    n1, TIFFFileName(tif1),
			    n2, TIFFFileName(tif2)
			);
	} else if (TIFFGetField(tif2, tag, &n2, &a2))
		printf("%s tag appears only in %s\n", name, TIFFFileName(tif2));
	else
		return (1);
	return (0);
}
