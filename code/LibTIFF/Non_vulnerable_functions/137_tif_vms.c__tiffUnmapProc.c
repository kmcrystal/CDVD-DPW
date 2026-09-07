static void
_tiffUnmapProc(thandle_t fd, tdata_t base, toff_t size)
{
	char *inadr[2];
	int i, j;
	
	for (i = 0;i < no_mapped; i++) {
		if (map_table[i].base == (char *) base) {
			
			inadr[0] = (char *) base;
			inadr[1] = map_table[i].top;
			sys$deltva(inadr, 0, 0);
			sys$dassgn(map_table[i].channel);
			
			for (j = i+1; j < no_mapped; j++)
				map_table[j-1] = map_table[j];
			no_mapped--;
			return;
		}
	}
}
static void
_tiffUnmapProc(thandle_t fd, tdata_t base, toff_t size)
{
}
