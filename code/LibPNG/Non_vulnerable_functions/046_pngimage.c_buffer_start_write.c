static void
buffer_start_write(struct buffer *buffer)
{
   buffer->last = &buffer->first;
   buffer->end_count = 0;
   buffer->current = NULL;
}
