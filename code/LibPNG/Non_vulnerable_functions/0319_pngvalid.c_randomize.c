static void
randomize(void *pv, size_t size)
{
   static png_uint_32 random_seed[2] = {0x56789abc, 0xd};
   make_random_bytes(random_seed, pv, size);
}
