static char
type_char(png_uint_32 v)
{
   





   if (v & 32)
      return "!abcdefghijklmnopqrstuvwxyz56789"[(v-96)&31];
   else
      return "@ABCDEFGHIJKLMNOPQRSTUVWXYZ01234"[(v-64)&31];
}
