}
void *png_far_to_near(png_structp png_ptr, png_voidp ptr, int check)
{
   void *near_ptr;
   void FAR *far_ptr;
   FP_OFF(near_ptr) = FP_OFF(ptr);
   far_ptr = (void FAR *)near_ptr;
   if (check != 0)
      if (FP_SEG(ptr) != FP_SEG(far_ptr))
         png_error(png_ptr, "segment lost in conversion");
   return(near_ptr);
}
}
void *png_far_to_near(png_structp png_ptr, png_voidp ptr, int check)
{
   void *near_ptr;
   void FAR *far_ptr;
   near_ptr = (void FAR *)ptr;
   far_ptr = (void FAR *)near_ptr;
   if (check != 0)
      if (far_ptr != ptr)
         png_error(png_ptr, "segment lost in conversion");
   return(near_ptr);
}
