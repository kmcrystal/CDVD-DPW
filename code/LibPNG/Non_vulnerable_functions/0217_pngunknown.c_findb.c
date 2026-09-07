static int
findb(const png_byte *name)
{
   int i = NINFO;
   while (--i >= 0)
   {
      if (memcmp(chunk_info[i].name, name, 4) == 0)
         break;
   }
   return i;
}
