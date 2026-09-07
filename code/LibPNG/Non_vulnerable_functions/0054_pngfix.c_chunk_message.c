static void
chunk_message(struct chunk *chunk, const char *message)
{
   type_message(chunk->file, chunk->chunk_type, message);
}
