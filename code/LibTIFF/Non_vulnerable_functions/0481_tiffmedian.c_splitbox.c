static void
splitbox(Colorbox* ptr)
{
	uint32		hist2[B_LEN];
	int		first=0, last=0;
	register Colorbox	*new;
	register uint32	*iptr, *histp;
	register int	i, j;
	register int	ir,ig,ib;
	register uint32 sum, sum1, sum2;
	enum { RED, GREEN, BLUE } axis;
	




	i = ptr->rmax - ptr->rmin;
	if (i >= ptr->gmax - ptr->gmin && i >= ptr->bmax - ptr->bmin)
		axis = RED;
	else if (ptr->gmax - ptr->gmin >= ptr->bmax - ptr->bmin)
		axis = GREEN;
	else
		axis = BLUE;
	
	switch (axis) {
	case RED:
		histp = &hist2[ptr->rmin];
	        for (ir = ptr->rmin; ir <= ptr->rmax; ++ir) {
			*histp = 0;
			for (ig = ptr->gmin; ig <= ptr->gmax; ++ig) {
				iptr = &histogram[ir][ig][ptr->bmin];
				for (ib = ptr->bmin; ib <= ptr->bmax; ++ib)
					*histp += *iptr++;
			}
			histp++;
	        }
	        first = ptr->rmin;
		last = ptr->rmax;
	        break;
	case GREEN:
	        histp = &hist2[ptr->gmin];
	        for (ig = ptr->gmin; ig <= ptr->gmax; ++ig) {
			*histp = 0;
			for (ir = ptr->rmin; ir <= ptr->rmax; ++ir) {
				iptr = &histogram[ir][ig][ptr->bmin];
				for (ib = ptr->bmin; ib <= ptr->bmax; ++ib)
					*histp += *iptr++;
			}
			histp++;
	        }
	        first = ptr->gmin;
		last = ptr->gmax;
	        break;
	case BLUE:
	        histp = &hist2[ptr->bmin];
	        for (ib = ptr->bmin; ib <= ptr->bmax; ++ib) {
			*histp = 0;
			for (ir = ptr->rmin; ir <= ptr->rmax; ++ir) {
				iptr = &histogram[ir][ptr->gmin][ib];
				for (ig = ptr->gmin; ig <= ptr->gmax; ++ig) {
					*histp += *iptr;
					iptr += B_LEN;
				}
			}
			histp++;
	        }
	        first = ptr->bmin;
		last = ptr->bmax;
	        break;
	}
	
	sum2 = ptr->total / 2;
	histp = &hist2[first];
	sum = 0;
	for (i = first; i <= last && (sum += *histp++) < sum2; ++i)
		;
	if (i == first)
		i++;
	
	new = freeboxes;
	freeboxes = new->next;
	if (freeboxes)
		freeboxes->prev = NULL;
	if (usedboxes)
		usedboxes->prev = new;
	new->next = usedboxes;
	usedboxes = new;
	histp = &hist2[first];
	for (sum1 = 0, j = first; j < i; j++)
		sum1 += *histp++;
	for (sum2 = 0, j = i; j <= last; j++)
	    sum2 += *histp++;
	new->total = sum1;
	ptr->total = sum2;
	new->rmin = ptr->rmin;
	new->rmax = ptr->rmax;
	new->gmin = ptr->gmin;
	new->gmax = ptr->gmax;
	new->bmin = ptr->bmin;
	new->bmax = ptr->bmax;
	switch (axis) {
	case RED:
		new->rmax = i-1;
	        ptr->rmin = i;
	        break;
	case GREEN:
	        new->gmax = i-1;
	        ptr->gmin = i;
	        break;
	case BLUE:
	        new->bmax = i-1;
	        ptr->bmin = i;
	        break;
	}
	shrinkbox(new);
	shrinkbox(ptr);
}
