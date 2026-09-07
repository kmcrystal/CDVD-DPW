static int
pcompar(const void* va, const void* vb)
{
    const int* pa = (const int*) va;
    const int* pb = (const int*) vb;
    return (*pa - *pb);
}
