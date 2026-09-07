}
static png_uint_32 png_exp(png_uint_32 x)
{
   return (png_uint_32)floor(.5 + exp(x * -LN2) * 0xffffffffU);
}
