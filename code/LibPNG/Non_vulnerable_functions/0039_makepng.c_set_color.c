static void
set_color(png_colorp color, png_bytep trans, unsigned int red,
   unsigned int green, unsigned int blue, unsigned int alpha,
   png_const_bytep gamma_table)
{
   color->red = gamma_table[red];
   color->green = gamma_table[green];
   color->blue = gamma_table[blue];
   *trans = (png_byte)alpha;
}
