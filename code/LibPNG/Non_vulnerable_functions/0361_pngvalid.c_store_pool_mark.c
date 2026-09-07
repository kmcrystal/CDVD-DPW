static void
store_pool_mark(png_bytep mark)
{
   static png_uint_32 store_seed[2] = { 0x12345678, 1};
   make_four_random_bytes(store_seed, mark);
}
