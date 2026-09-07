static void
buffer_destroy(struct buffer *buffer)
{
   struct buffer_list *list = buffer->first.next;
   buffer_init(buffer);
   buffer_destroy_list(list);
}
