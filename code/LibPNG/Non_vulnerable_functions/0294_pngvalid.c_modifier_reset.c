static void
modifier_reset(png_modifier *pm)
{
   store_read_reset(&pm->this);
   pm->limit = 4E-3;
   pm->pending_len = pm->pending_chunk = 0;
   pm->flush = pm->buffer_count = pm->buffer_position = 0;
   pm->modifications = NULL;
   pm->state = modifier_start;
   modifier_encoding_iterate(pm);
   



   pm->test_uses_encoding = 0;
   pm->current_gamma = 0;
   pm->current_encoding = 0;
   pm->encoding_ignored = 0;
   
   pm->bit_depth = pm->colour_type = 0;
}
