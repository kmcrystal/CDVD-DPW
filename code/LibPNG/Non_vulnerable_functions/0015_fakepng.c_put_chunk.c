void
put_chunk(const unsigned char *chunk, uInt length)
{
   uLong crc;
   put_uLong(length-4); 
   fwrite(chunk, length, 1, stdout);
   crc = crc32(0, Z_NULL, 0);
   put_uLong(crc32(crc, chunk, length));
}
