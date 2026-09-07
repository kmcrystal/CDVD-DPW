static double
chromaticity_x(CIE_color c)
{
   return c.X / (c.X + c.Y + c.Z);
}
