static unsigned int
sample(png_const_bytep row, png_byte colour_type, png_byte bit_depth,
    png_uint_32 x, unsigned int sample_index)
{
   png_uint_32 bit_index, result;
   
   x *= bit_depth;
   bit_index = x;
   if ((colour_type & 1) == 0) 
   {
      if (colour_type & 2)
         bit_index *= 3;
      if (colour_type & 4)
         bit_index += x; 
      
      if (colour_type & (2+4))
         bit_index += sample_index * bit_depth;
   }
   
   row += bit_index >> 3;
   result = *row;
   if (bit_depth == 8)
      return result;
   else if (bit_depth > 8)
      return (result << 8) + *++row;
   
   bit_index &= 7;
   return (result >> (8-bit_index-bit_depth)) & ((1U<<bit_depth)-1);
}
