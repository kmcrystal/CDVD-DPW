static void
random_color(png_colorp color)
{
   static png_uint_32 color_seed[2] = { 0x12345678, 0x9abcdef };
   make_random_bytes(color_seed, color, sizeof *color);
}
