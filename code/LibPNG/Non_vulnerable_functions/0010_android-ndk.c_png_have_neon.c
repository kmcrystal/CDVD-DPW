static int
png_have_neon(png_structp png_ptr)
{
   



   PNG_UNUSED(png_ptr)
   return android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM &&
      (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
}
