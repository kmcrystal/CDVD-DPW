static int
ancillaryb(const png_byte *name)
{
   return PNG_CHUNK_ANCILLARY(PNG_U32(name[0], name[1], name[2], name[3]));
}
