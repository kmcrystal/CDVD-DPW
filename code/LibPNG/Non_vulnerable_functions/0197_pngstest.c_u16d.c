static png_uint_16
u16d(double d)
{
   d = closestinteger(d);
   return (png_uint_16)d;
}
