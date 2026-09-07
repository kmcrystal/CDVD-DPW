static void
type_name(png_uint_32 type, FILE *out)
{
   putc(type_char(type >> 24), out);
   putc(type_char(type >> 16), out);
   putc(type_char(type >>  8), out);
   putc(type_char(type      ), out);
}
