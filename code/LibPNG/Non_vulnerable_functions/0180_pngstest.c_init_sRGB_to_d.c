static void
init_sRGB_to_d(void)
{
   int i;
   sRGB_to_d[0] = 0;
   for (i=1; i<255; ++i)
      sRGB_to_d[i] = linear_from_sRGB(i/255.);
   sRGB_to_d[255] = 1;
   g22_to_d[0] = 0;
   for (i=1; i<255; ++i)
      g22_to_d[i] = pow(i/255., 1/.45455);
   g22_to_d[255] = 1;
}
