static void
processZIPOptions(char* cp)
{
	if ( (cp = strchr(cp, ':')) ) {
		do {
			cp++;
			if (isdigit((int)*cp))
				defpredictor = atoi(cp);
			else if (*cp == 'p')
				defpreset = atoi(++cp);
			else
				usage();
		} while( (cp = strchr(cp, ':')) );
	}
}
