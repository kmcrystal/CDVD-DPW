static C_cell *
create_colorcell(int red, int green, int blue)
{
	register int ir, ig, ib, i;
	register C_cell *ptr;
	int mindist, next_n;
	register int tmp, dist, n;
	ir = red >> (COLOR_DEPTH-C_DEPTH);
	ig = green >> (COLOR_DEPTH-C_DEPTH);
	ib = blue >> (COLOR_DEPTH-C_DEPTH);
	ptr = (C_cell *)_TIFFmalloc(sizeof (C_cell));
	*(ColorCells + ir*C_LEN*C_LEN + ig*C_LEN + ib) = ptr;
	ptr->num_ents = 0;
	



	mindist = 99999999;
	for (i = 0; i < num_colors; ++i) {
		if (rm[i]>>(COLOR_DEPTH-C_DEPTH) != ir  ||
		    gm[i]>>(COLOR_DEPTH-C_DEPTH) != ig  ||
		    bm[i]>>(COLOR_DEPTH-C_DEPTH) != ib)
			continue;
		ptr->entries[ptr->num_ents][0] = i;
		ptr->entries[ptr->num_ents][1] = 0;
		++ptr->num_ents;
	        tmp = rm[i] - red;
	        if (tmp < (MAX_COLOR/C_LEN/2))
			tmp = MAX_COLOR/C_LEN-1 - tmp;
	        dist = tmp*tmp;
	        tmp = gm[i] - green;
	        if (tmp < (MAX_COLOR/C_LEN/2))
			tmp = MAX_COLOR/C_LEN-1 - tmp;
	        dist += tmp*tmp;
	        tmp = bm[i] - blue;
	        if (tmp < (MAX_COLOR/C_LEN/2))
			tmp = MAX_COLOR/C_LEN-1 - tmp;
	        dist += tmp*tmp;
	        if (dist < mindist)
			mindist = dist;
	}
	


	for (i = 0; i < num_colors; ++i) {
		if (rm[i] >> (COLOR_DEPTH-C_DEPTH) == ir  &&
		    gm[i] >> (COLOR_DEPTH-C_DEPTH) == ig  &&
		    bm[i] >> (COLOR_DEPTH-C_DEPTH) == ib)
			continue;
		dist = 0;
	        if ((tmp = red - rm[i]) > 0 ||
		    (tmp = rm[i] - (red + MAX_COLOR/C_LEN-1)) > 0 )
			dist += tmp*tmp;
	        if ((tmp = green - gm[i]) > 0 ||
		    (tmp = gm[i] - (green + MAX_COLOR/C_LEN-1)) > 0 )
			dist += tmp*tmp;
	        if ((tmp = blue - bm[i]) > 0 ||
		    (tmp = bm[i] - (blue + MAX_COLOR/C_LEN-1)) > 0 )
			dist += tmp*tmp;
	        if (dist < mindist) {
			ptr->entries[ptr->num_ents][0] = i;
			ptr->entries[ptr->num_ents][1] = dist;
			++ptr->num_ents;
	        }
	}
	


	for (n = ptr->num_ents - 1; n > 0; n = next_n) {
		next_n = 0;
		for (i = 0; i < n; ++i)
			if (ptr->entries[i][1] > ptr->entries[i+1][1]) {
				tmp = ptr->entries[i][0];
				ptr->entries[i][0] = ptr->entries[i+1][0];
				ptr->entries[i+1][0] = tmp;
				tmp = ptr->entries[i][1];
				ptr->entries[i][1] = ptr->entries[i+1][1];
				ptr->entries[i+1][1] = tmp;
				next_n = i;
		        }
	}
	return (ptr);
}
