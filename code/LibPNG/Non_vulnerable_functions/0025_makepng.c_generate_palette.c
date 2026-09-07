static int
generate_palette(png_colorp palette, png_bytep trans, int bit_depth,
   png_const_bytep gamma_table, unsigned int *colors)
{
   








   switch (colors[0])
   {
      default:
         fprintf(stderr, "makepng: --colors=...: invalid count %u\n",
            colors[0]);
         exit(1);
      case 1:
         set_color(palette+0, trans+0, colors[1], colors[1], colors[1], 255,
            gamma_table);
         return 1;
      case 2:
         set_color(palette+0, trans+0, colors[1], colors[1], colors[1],
            colors[2], gamma_table);
         return 1;
      case 3:
         set_color(palette+0, trans+0, colors[1], colors[2], colors[3], 255,
            gamma_table);
         return 1;
      case 4:
         set_color(palette+0, trans+0, colors[1], colors[2], colors[3],
            colors[4], gamma_table);
         return 1;
      case 0:
         if (bit_depth == 1)
         {
            set_color(palette+0, trans+0, 255, 0, 0, 0, gamma_table);
            set_color(palette+1, trans+1, 255, 255, 255, 255, gamma_table);
            return 2;
         }
         else
         {
            unsigned int size = 1U << (bit_depth/2); 
            unsigned int x, y, ip;
            for (x=0; x<size; ++x) for (y=0; y<size; ++y)
            {
               ip = x + (size * y);
               

#              define interp(pos, c1, c2) ((pos * c1) + ((size-pos) * c2))
#              define xyinterp(x, y, c1, c2, c3, c4) (((size * size / 2) +\
                  (interp(x, c1, c2) * y + (size-y) * interp(x, c3, c4))) /\
                  (size*size))
               set_color(palette+ip, trans+ip,
                  
                  xyinterp(x, y,   0, 255,   0, 255),
                  xyinterp(x, y, 255,   0,   0, 255),
                  xyinterp(x, y,   0,   0, 255, 255),
                  
                  xyinterp(x, y,   0, 102, 204, 255),
                  gamma_table);
            }
            return ip+1;
         }
   }
}
