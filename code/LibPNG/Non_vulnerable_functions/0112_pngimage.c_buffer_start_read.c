static void
buffer_start_read(struct buffer *buffer)
{
   buffer->current = &buffer->first;
   buffer->read_count = 0;
}
