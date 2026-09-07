static void
PrintTag(FILE* fd, uint16 tag)
{
	const struct tagname *tp;
	for (tp = tagnames; tp < &tagnames[NTAGS]; tp++)
		if (tp->tag == tag) {
			fprintf(fd, "%s (%u)", tp->name, tag);
			return;
		}
	fprintf(fd, "%u (%#x)", tag, tag);
}
