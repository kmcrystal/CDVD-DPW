int
main(int argc, char* argv[])
{
	int one = 1, fd;
	int multiplefiles = (argc > 1);
	int c;
	uint64 diroff = 0;
	bigendian = (*(char *)&one == 0);
	appname = argv[0];
	while ((c = getopt(argc, argv, "m:o:h")) != -1) {
		switch (c) {
		case 'h':			
			shortfmt = "%s%#x";
			sshortfmt = "%s%#x";
			longfmt = "%s%#lx";
			slongfmt = "%s%#lx";
			break;
		case 'o':
			diroff = (uint64) strtoul(optarg, NULL, 0);
			break;
		case 'm':
			maxitems = strtoul(optarg, NULL, 0);
			break;
		default:
			usage();
		}
	}
	if (optind >= argc)
		usage();
	for (; optind < argc; optind++) {
		fd = open(argv[optind], O_RDONLY|O_BINARY, 0);
		if (fd < 0) {
			perror(argv[0]);
			return (-1);
		}
		if (multiplefiles)
			printf("%s:\n", argv[optind]);
		curfile = argv[optind];
		swabflag = 0;
		bigtiff = 0;
		dump(fd, diroff);
		close(fd);
	}
	return (0);
}
