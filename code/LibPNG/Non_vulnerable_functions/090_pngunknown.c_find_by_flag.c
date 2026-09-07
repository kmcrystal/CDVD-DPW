static int
find_by_flag(png_uint_32 flag)
{
   int i = NINFO;
   while (--i >= 0) if (chunk_info[i].flag == flag) return i;
   fprintf(stderr, "pngunknown: internal error\n");
   exit(4);
}
