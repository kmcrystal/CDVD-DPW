static double
chromaticity_y(CIE_color c)
{
   return c.Y / (c.X + c.Y + c.Z);
}
