static png_byte
u8d(double d)
{
   d = closestinteger(d);
   return (png_byte)d;
}
