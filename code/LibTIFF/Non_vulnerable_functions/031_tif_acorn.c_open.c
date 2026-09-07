
extern int open(const char* name, int flags, int mode)
{
	
	os_error* e = (os_error*) 1; 
	os_f file = (os_f) -1;
	flags = flags;
	switch(mode)
	{
		case O_RDONLY:
		{
			e = xosfind_openin(SKIP, name, SKIP, &file);
			break;
		}
		case O_WRONLY:
		case O_RDWR|O_CREAT:
		case O_RDWR|O_CREAT|O_TRUNC:
		{
			e = xosfind_openout(SKIP, name, SKIP, &file);
			break;
		}
		case O_RDWR:
		{
			e = xosfind_openup(SKIP, name, SKIP, &file);
			break;
		}
	}
	if (e)
	{
		file = (os_f) -1;
	}
	return (file);
}
