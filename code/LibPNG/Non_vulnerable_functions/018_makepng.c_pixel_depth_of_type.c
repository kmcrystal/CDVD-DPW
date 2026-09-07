static int
pixel_depth_of_type(int color_type, int bit_depth)
{
   return channels_of_type(color_type) * bit_depth;
}
