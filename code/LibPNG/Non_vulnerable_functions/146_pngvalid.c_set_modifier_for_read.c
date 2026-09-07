static png_structp
set_modifier_for_read(png_modifier *pm, png_infopp ppi, png_uint_32 id,
    PNG_CONST char *name)
{
   



   pm->state = modifier_start;
   pm->bit_depth = 0;
   pm->colour_type = 255;
   pm->pending_len = 0;
   pm->pending_chunk = 0;
   pm->flush = 0;
   pm->buffer_count = 0;
   pm->buffer_position = 0;
   return set_store_for_read(&pm->this, ppi, id, name);
}
