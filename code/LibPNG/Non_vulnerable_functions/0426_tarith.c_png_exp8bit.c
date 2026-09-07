}
static png_byte png_exp8bit(png_uint_32 log)
{
   return (png_byte)floor(.5 + exp(log * -LN2) * 255);
}
