static void
standard_info_imp(standard_display *dp, png_structp pp, png_infop pi,
    int nImages)
{
   


   standard_info_part1(dp, pp, pi);
   


   if (dp->use_update_info)
   {
      
      int i = dp->use_update_info;
      while (i-- > 0)
         png_read_update_info(pp, pi);
   }
   else
      png_start_read_image(pp);
   


   standard_info_part2(dp, pp, pi, nImages);
}
