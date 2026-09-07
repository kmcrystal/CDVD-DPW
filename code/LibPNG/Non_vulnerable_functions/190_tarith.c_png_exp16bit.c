}
static png_uint_16 png_exp16bit(png_uint_32 log)
{
   return (png_uint_16)floor(.5 + exp(log * -LN2) * 65535);
}
