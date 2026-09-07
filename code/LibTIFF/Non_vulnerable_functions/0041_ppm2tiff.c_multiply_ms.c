static tmsize_t
multiply_ms(tmsize_t m1, tmsize_t m2)
{
	tmsize_t bytes = m1 * m2;
	if (m1 && bytes / m1 != m2)
		bytes = 0;
	return bytes;
}
