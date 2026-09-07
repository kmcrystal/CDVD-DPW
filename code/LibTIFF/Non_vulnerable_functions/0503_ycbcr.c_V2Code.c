static unsigned
V2Code(float f, unsigned long RB, unsigned long RW, int CR)
{
    unsigned int c = (unsigned int)((((f)*(RW-RB)/CR)+RB)+.5);
    return (c > 255 ? 255 : c);
}
