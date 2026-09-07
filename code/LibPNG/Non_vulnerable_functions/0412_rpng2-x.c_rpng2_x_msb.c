}
static int rpng2_x_msb(ulg u32val)
{
    int i;
    for (i = 31;  i >= 0;  --i) {
        if (u32val & 0x80000000L)
            break;
        u32val <<= 1;
    }
    return i;
}
