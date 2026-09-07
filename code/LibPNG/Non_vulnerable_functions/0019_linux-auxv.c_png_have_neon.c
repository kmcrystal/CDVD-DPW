static int
png_have_neon(png_structp png_ptr)
{
   int fd = open("/proc/self/auxv", O_RDONLY);
   Elf32_auxv_t aux;
   
   if (fd == -1)
   {
      png_warning(png_ptr, "/proc/self/auxv open failed");
      return 0;
   }
   while (safe_read(png_ptr, fd, &aux, sizeof aux) == sizeof aux)
   {
      if (aux.a_type == AT_HWCAP && (aux.a_un.a_val & HWCAP_NEON) != 0)
      {
         close(fd);
         return 1;
      }
   }
   close(fd);
   return 0;
}
