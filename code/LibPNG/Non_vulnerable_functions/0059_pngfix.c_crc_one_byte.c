static png_uint_32
crc_one_byte(png_uint_32 crc, int b)
{
   return crc_table[(crc ^ b) & 0xff] ^ (crc >> 8);
}
