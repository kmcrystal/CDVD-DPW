static void
clear(void *pv, size_t size)
{
   static png_uint_32 clear_seed[2] = { 0x12345678, 0x9abcdef0 };
   make_random_bytes(clear_seed, pv, size);
}
