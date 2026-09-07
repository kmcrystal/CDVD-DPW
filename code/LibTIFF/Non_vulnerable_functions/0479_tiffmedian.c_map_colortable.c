static void
map_colortable(void)
{
	register uint32 *histp = &histogram[0][0][0];
	register C_cell *cell;
	register int j, tmp, d2, dist;
	int ir, ig, ib, i;
	for (ir = 0; ir < B_LEN; ++ir)
		for (ig = 0; ig < B_LEN; ++ig)
			for (ib = 0; ib < B_LEN; ++ib, histp++) {
				if (*histp == 0) {
					*histp = -1;
					continue;
				}
				cell = *(ColorCells +
				    (((ir>>(B_DEPTH-C_DEPTH)) << C_DEPTH*2) +
				    ((ig>>(B_DEPTH-C_DEPTH)) << C_DEPTH) +
				    (ib>>(B_DEPTH-C_DEPTH))));
				if (cell == NULL )
					cell = create_colorcell(
					    ir << COLOR_SHIFT,
					    ig << COLOR_SHIFT,
					    ib << COLOR_SHIFT);
				dist = 9999999;
				for (i = 0; i < cell->num_ents &&
				    dist > cell->entries[i][1]; ++i) {
					j = cell->entries[i][0];
					d2 = rm[j] - (ir << COLOR_SHIFT);
					d2 *= d2;
					tmp = gm[j] - (ig << COLOR_SHIFT);
					d2 += tmp*tmp;
					tmp = bm[j] - (ib << COLOR_SHIFT);
					d2 += tmp*tmp;
					if (d2 < dist) {
						dist = d2;
						*histp = j;
					}
				}
			}
}
