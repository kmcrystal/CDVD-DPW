int
getopt(int argc, char * const argv[], const char *optstring)
{
	static char *place = EMSG;		
	char *oli;				
	if (optreset || *place == 0) {		
		optreset = 0;
		place = argv[optind];
		if (optind >= argc || *place++ != '-') {
			
			place = EMSG;
			return (-1);
		}
		optopt = *place++;
		if (optopt == '-' && *place == 0) {
			
			++optind;
			place = EMSG;
			return (-1);
		}
		if (optopt == 0) {
			

			place = EMSG;
			if (strchr(optstring, '-') == NULL)
				return -1;
			optopt = '-';
		}
	} else
		optopt = *place++;
	
	if (optopt == ':' || (oli = strchr(optstring, optopt)) == NULL) {
		if (*place == 0)
			++optind;
		if (opterr && *optstring != ':')
			(void)fprintf(stderr,
                                      "unknown option -- %c\n", optopt);
		return (BADCH);
	}
	
	if (oli[1] != ':') {
		
		optarg = NULL;
		if (*place == 0)
			++optind;
	} else {
		

		if (*place)
			optarg = place;
		else if (argc > ++optind)
			optarg = argv[optind];
		else {
			
			place = EMSG;
			if (*optstring == ':')
				return (BADARG);
			if (opterr)
				(void)fprintf(stderr,
                                        "option requires an argument -- %c\n",
                                        optopt);
			return (BADCH);
		}
		place = EMSG;
		++optind;
	}
	return (optopt);			
}
