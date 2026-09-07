static void
chunk_end(struct chunk **chunk_var)
{
   struct chunk *chunk = *chunk_var;
   *chunk_var = NULL;
   CLEAR(*chunk);
}
