int
main(void)
{
    int failure = 0;
    failure |= test_packbits();
    
    failure |= rewrite_test( "rewrite1.tif", 10, 0, 100 );
    failure |= rewrite_test( "rewrite2.tif", 10, 1, 100 );
    
    failure |= rewrite_test( "rewrite3.tif", 1, 0, 100 );
    failure |= rewrite_test( "rewrite4.tif", 1, 1, 100 );
    
    failure |= rewrite_test( "rewrite5.tif", 1000, 1, 0x6000000000ULL );
    failure |= rewrite_test( "rewrite6.tif", 1, 1, 0x6000000000ULL );
    return failure;
}
