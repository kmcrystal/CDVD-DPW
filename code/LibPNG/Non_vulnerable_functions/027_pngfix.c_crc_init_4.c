static png_uint_32
crc_init_4(png_uint_32 value)
{
   






   png_uint_32 crc = crc_table[(~value >> 24)] ^ 0xffffff;
   crc = crc_table[(crc ^ (value >> 16)) & 0xff] ^ (crc >> 8);
   crc = crc_table[(crc ^ (value >> 8)) & 0xff] ^ (crc >> 8);
   return crc_table[(crc ^ value) & 0xff] ^ (crc >> 8);
}
