static void
modifier_setbuffer(png_modifier *pm)
{
   modifier_crc(pm->buffer);
   pm->buffer_count = png_get_uint_32(pm->buffer)+12;
   pm->buffer_position = 0;
}
