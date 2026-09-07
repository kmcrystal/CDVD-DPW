static void
PrintType(FILE* fd, uint16 type)
{
	static const char *typenames[] = {
	    "0",
	    "BYTE",
	    "ASCII",
	    "SHORT",
	    "LONG",
	    "RATIONAL",
	    "SBYTE",
	    "UNDEFINED",
	    "SSHORT",
	    "SLONG",
	    "SRATIONAL",
	    "FLOAT",
	    "DOUBLE",
	    "IFD",
	    "14",
	    "15",
	    "LONG8",
	    "SLONG8",
	    "IFD8"
	};
#define	NTYPES	(sizeof (typenames) / sizeof (typenames[0]))
	if (type < NTYPES)
		fprintf(fd, "%s (%u)", typenames[type], type);
	else
		fprintf(fd, "%u (%#x)", type, type);
}
