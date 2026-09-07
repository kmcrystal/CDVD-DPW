static tmsize_t
multiply_ms(tmsize_t m1, tmsize_t m2)
{
        if( m1 == 0 || m2 > TIFF_TMSIZE_T_MAX / m1 )
            return 0;
        return m1 * m2;
}
