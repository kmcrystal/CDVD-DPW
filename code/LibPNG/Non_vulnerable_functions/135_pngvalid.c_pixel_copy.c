static void
pixel_copy(png_bytep toBuffer, png_uint_32 toIndex,
   png_const_bytep fromBuffer, png_uint_32 fromIndex, unsigned int pixelSize)
{
   


   toIndex *= pixelSize;
   fromIndex *= pixelSize;
   if (pixelSize < 8) 
   {
      
      unsigned int destMask = ((1U<<pixelSize)-1) << (8-pixelSize-(toIndex&7));
      
      unsigned int destByte = toBuffer[toIndex >> 3] & ~destMask;
      unsigned int sourceByte = fromBuffer[fromIndex >> 3];
      
      fromIndex &= 7;
      if (fromIndex > 0) sourceByte <<= fromIndex;
      if ((toIndex & 7) > 0) sourceByte >>= toIndex & 7;
      toBuffer[toIndex >> 3] = (png_byte)(destByte | (sourceByte & destMask));
   }
   else 
      memmove(toBuffer+(toIndex>>3), fromBuffer+(fromIndex>>3), pixelSize>>3);
}
