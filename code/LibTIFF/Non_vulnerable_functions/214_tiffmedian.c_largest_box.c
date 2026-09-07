static Colorbox *
largest_box(void)
{
	register Colorbox *p, *b;
	register uint32 size;
	b = NULL;
	size = 0;
	for (p = usedboxes; p != NULL; p = p->next)
		if ((p->rmax > p->rmin || p->gmax > p->gmin ||
		    p->bmax > p->bmin) &&  p->total > size)
		        size = (b = p)->total;
	return (b);
}
