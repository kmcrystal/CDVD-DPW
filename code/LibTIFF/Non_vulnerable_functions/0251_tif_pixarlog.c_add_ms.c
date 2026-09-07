static tmsize_t
add_ms(tmsize_t m1, tmsize_t m2)
{
	
	if (m1 == 0 || m2 == 0)
		return 0;
	else if (m1 > TIFF_TMSIZE_T_MAX - m2)
		return 0;
	return m1 + m2;
}
