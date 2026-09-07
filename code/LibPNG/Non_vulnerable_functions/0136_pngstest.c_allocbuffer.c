static void
allocbuffer(Image *image)
{
   png_size_t size = PNG_IMAGE_BUFFER_SIZE(image->image, image->stride);
   if (size+32 > image->bufsize)
   {
      freebuffer(image);
      image->buffer = voidcast(png_bytep, malloc(size+32));
      if (image->buffer == NULL)
      {
         fflush(stdout);
         fprintf(stderr,
            "simpletest: out of memory allocating %lu(+32) byte buffer\n",
            (unsigned long)size);
         exit(1);
      }
      image->bufsize = size+32;
   }
   memset(image->buffer, 95, image->bufsize);
   memset(image->buffer+16, BUFFER_INIT8, size);
   image->allocsize = size;
}
