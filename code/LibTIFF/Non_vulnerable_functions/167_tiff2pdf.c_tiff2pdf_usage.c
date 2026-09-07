void tiff2pdf_usage(){
	char* lines[]={
	"usage:  tiff2pdf [options] input.tiff",
	"options:",
	" -o: output to file name",
#ifdef JPEG_SUPPORT
	" -j: compress with JPEG", 
#endif
#ifdef ZIP_SUPPORT
	" -z: compress with Zip/Deflate",
#endif
	" -q: compression quality",
	" -n: no compressed data passthrough",
	" -d: do not compress (decompress)",
	" -i: invert colors",
	" -u: set distance unit, 'i' for inch, 'm' for centimeter",
	" -x: set x resolution default in dots per unit",
	" -y: set y resolution default in dots per unit",
	" -w: width in units",
	" -l: length in units",
	" -r: 'd' for resolution default, 'o' for resolution override",
	" -p: paper size, eg \"letter\", \"legal\", \"A4\"",
  " -F: make the tiff fill the PDF page",
	" -f: set PDF \"Fit Window\" user preference",
	" -e: date, overrides image or current date/time default, YYYYMMDDHHMMSS",
	" -c: sets document creator, overrides image software default",
	" -a: sets document author, overrides image artist default",
	" -t: sets document title, overrides image document name default",
	" -s: sets document subject, overrides image image description default",
	" -k: sets document keywords",
	" -b: set PDF \"Interpolate\" user preference",
	" -h: usage",
	NULL
	};
	int i=0;
	fprintf(stderr, "%s\n\n", TIFFGetVersion());
	for (i=0;lines[i]!=NULL;i++){
		fprintf(stderr, "%s\n", lines[i]);
	}
	return;
}
}
void tiff2pdf_usage(){
	char* lines[]={
	"usage:  tiff2pdf [options] input.tiff",
	"options:",
	" -o: output to file name",
#ifdef JPEG_SUPPORT
	" -j: compress with JPEG", 
#endif
#ifdef ZIP_SUPPORT
	" -z: compress with Zip/Deflate",
#endif
	" -q: compression quality",
	" -n: no compressed data passthrough",
	" -d: do not compress (decompress)",
	" -i: invert colors",
	" -u: set distance unit, 'i' for inch, 'm' for centimeter",
	" -x: set x resolution default in dots per unit",
	" -y: set y resolution default in dots per unit",
	" -w: width in units",
	" -l: length in units",
	" -r: 'd' for resolution default, 'o' for resolution override",
	" -p: paper size, eg \"letter\", \"legal\", \"A4\"",
  " -F: make the tiff fill the PDF page",
	" -f: set PDF \"Fit Window\" user preference",
	" -e: date, overrides image or current date/time default, YYYYMMDDHHMMSS",
	" -c: sets document creator, overrides image software default",
	" -a: sets document author, overrides image artist default",
	" -t: sets document title, overrides image document name default",
	" -s: sets document subject, overrides image image description default",
	" -k: sets document keywords",
	" -b: set PDF \"Interpolate\" user preference",
	" -h: usage",
	NULL
	};
	int i=0;
	fprintf(stderr, "%s\n\n", TIFFGetVersion());
	for (i=0;lines[i]!=NULL;i++){
		fprintf(stderr, "%s\n", lines[i]);
	}
	return;
}
