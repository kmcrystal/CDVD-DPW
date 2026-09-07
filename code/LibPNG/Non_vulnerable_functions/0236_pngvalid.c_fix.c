static png_fixed_point
fix(double d)
{
   d = floor(d * PNG_FP_1 + .5);
   return (png_fixed_point)d;
}
