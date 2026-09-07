static void
size_row(png_byte buffer[SIZE_ROWMAX], png_uint_32 bit_width, png_uint_32 y)
{
   
   y = ((y & 1) << 7) + ((y & 2) << 6) + ((y & 4) << 5) + ((y & 8) << 4);
   
   y ^= 0xA5;
   while (bit_width >= 8)
      *buffer++ = (png_byte)y++, bit_width -= 8;
   


   if (bit_width > 0)
   {
      png_uint_32 mask = (1U<<(8-bit_width))-1;
      *buffer = (png_byte)((*buffer & mask) | (y & ~mask));
   }
}
