static void
freebuffer(Image *image)
{
   if (image->buffer) free(image->buffer);
   image->buffer = NULL;
   image->bufsize = 0;
   image->allocsize = 0;
}
